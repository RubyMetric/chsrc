#!/usr/bin/env python3
"""Isolated environment integration tests; no third-party dependencies.

Run after building chsrc and fw: python3 test/env.py
Optional binary paths: CHSRC_TEST_BIN and CHSRC_TEST_FW.
"""

import os
import shutil
from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parent.parent
BIN = Path(os.environ.get("CHSRC_TEST_BIN", str(ROOT / "chsrc"))).resolve()
FW = Path(os.environ.get("CHSRC_TEST_FW", str(ROOT / "fw"))).resolve()


@unittest.skipIf(os.name == "nt", "POSIX file backend; never modify Windows Registry")
class EnvironmentTests(unittest.TestCase):
    def setUp(self):
        self.sandbox = tempfile.TemporaryDirectory(prefix="chsrc-env-test-")
        self.addCleanup(self.sandbox.cleanup)
        self.home = Path(self.sandbox.name)
        self.env = os.environ.copy()
        for name in ("PUB_HOSTED_URL", "RUSTUP_DIST_SERVER", "RUSTUP_UPDATE_ROOT",
                     "XDG_CONFIG_HOME", "ZDOTDIR", "BASH_ENV", "ENV"):
            self.env.pop(name, None)
        self.env.update(HOME=str(self.home), SHELL="/bin/bash")

    def run_cli(self, *args):
        return subprocess.run([str(BIN), *args], cwd=ROOT, env=self.env,
                              capture_output=True, text=True, timeout=30)

    def test_framework_preserves_callers_home(self):
        fixtures = {
            ".bashrc": b"# original bashrc\n",
            ".profile": b"# original profile\n",
            ".config/fish/config.fish": b"# original fish config\n",
            ".config/fish/conf.d/chsrc.fish": b"# original fish snippet\n",
        }
        for name, data in fixtures.items():
            path = self.home / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(data)
        result = subprocess.run([str(FW)], cwd=ROOT, env=self.env,
                                capture_output=True, text=True, timeout=60)
        # Check the original files even if the framework process failed.
        for name, data in fixtures.items():
            self.assertEqual((self.home / name).read_bytes(), data, name)
        self.assertEqual(sorted(str(p.relative_to(self.home)) for p in self.home.rglob("*") if p.is_file()),
                         sorted(fixtures))
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_complex_shell_definitions_are_unchanged(self):
        cases = [
            'if false; then\n  export PUB_HOSTED_URL=old\nelse\n  export PUB_HOSTED_URL=fallback\nfi\n',
            "cat <<'EOF' >/dev/null\nexport PUB_HOSTED_URL=example\nEOF\nexport PUB_HOSTED_URL=old\n",
            "message='hello\nexport PUB_HOSTED_URL=example\n'\nexport PUB_HOSTED_URL=old\n",
            'configure() {\nexport PUB_HOSTED_URL=old\n}\n',
            'printf "%s" \\\nexport PUB_HOSTED_URL=example\n',
            'if true; then export PUB_HOSTED_URL=old; fi\n',
        ]
        for content in cases:
            with self.subTest(content=content):
                path = self.home / '.bashrc'
                path.write_text(content)
                before = sorted(p.name for p in self.home.iterdir())
                for action in ('set', 'reset', 'get'):
                    args = [action, '-en', '-no-color', 'pub']
                    if action == 'set':
                        args.append('tuna')
                    result = self.run_cli(*args)
                    self.assertNotEqual(result.returncode, 0, result.stdout)
                    self.assertIn('Cannot safely resolve', result.stderr)
                    self.assertEqual(path.read_text(), content)
                    self.assertEqual(sorted(p.name for p in self.home.iterdir()), before)

    def test_unrelated_compound_command_allows_top_level_assignment(self):
        original = ('if true; then\n  printf "ok" >/dev/null\nfi\n'
                    'colors="$(printf \'%s\' "blue")"\n'
                    'day=`printf Monday`\n')
        path = self.home / '.bashrc'
        path.write_text(original + 'export PUB_HOSTED_URL=old\n')
        result = self.run_cli('set', '-en', '-no-color', 'pub', 'tuna')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(path.read_text(), original +
                         'export PUB_HOSTED_URL="https://mirrors.tuna.tsinghua.edu.cn/dart-pub"\n')

    def test_fish_conditional_is_preserved(self):
        self.env['SHELL'] = '/usr/bin/fish'
        path = self.home / '.config/fish/config.fish'
        path.parent.mkdir(parents=True)
        content = 'if false\nset -gx PUB_HOSTED_URL old\nelse\nset -gx PUB_HOSTED_URL fallback\nend\n'
        path.write_text(content)
        result = self.run_cli('set', '-en', '-no-color', 'pub', 'tuna')
        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(path.read_text(), content)
        self.assertFalse((path.parent / 'conf.d/chsrc.fish').exists())

    @unittest.skipIf(hasattr(os, 'geteuid') and os.geteuid() == 0, 'root bypasses file permissions')
    def test_readonly_second_file_leaves_all_configuration_unchanged(self):
        original = b'export PUB_HOSTED_URL="https://old.example"\n'
        for name in ('.bashrc', '.profile'):
            (self.home / name).write_bytes(original)
        profile = self.home / '.profile'
        profile.chmod(0o444)
        self.addCleanup(profile.chmod, 0o644)
        result = self.run_cli('set', '-en', '-no-color', 'pub', 'tuna')
        self.assertNotEqual(result.returncode, 0)
        for name in ('.bashrc', '.profile'):
            self.assertEqual((self.home / name).read_bytes(), original)
        self.assertEqual(sorted(p.name for p in self.home.iterdir()), ['.bashrc', '.profile'])

    def test_atomic_update_preserves_symlink_and_permissions(self):
        target = self.home / 'actual-config'
        target.write_text('export PUB_HOSTED_URL=old\n')
        target.chmod(0o640)
        rc = self.home / '.bashrc'
        rc.symlink_to(target)
        result = self.run_cli('set', '-en', '-no-color', 'pub', 'tuna')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertTrue(rc.is_symlink())
        self.assertEqual(target.stat().st_mode & 0o777, 0o640)
        self.assertEqual(target.read_text(),
                         'export PUB_HOSTED_URL="https://mirrors.tuna.tsinghua.edu.cn/dart-pub"\n')
        self.assertEqual(list(self.home.glob('*.chsrc-*')), [])

    def test_bash_login_file_is_updated_and_loaded(self):
        login = self.home / '.bash_login'
        login.write_text('export PUB_HOSTED_URL="https://old.example"\n')
        result = self.run_cli('set', '-en', '-no-color', 'pub', 'tuna')
        self.assertEqual(result.returncode, 0, result.stderr)
        actual = subprocess.run(['bash', '--login', '-c', 'printenv PUB_HOSTED_URL'],
                                env=self.env, capture_output=True, text=True, timeout=10)
        self.assertEqual(actual.stdout.strip(), 'https://mirrors.tuna.tsinghua.edu.cn/dart-pub')
        self.assertFalse((self.home / '.bash_profile').exists())
        self.assertFalse((self.home / '.profile').exists())
        self.assertTrue((self.home / '.bashrc').exists())

    def test_bash_query_ignores_inactive_login_files(self):
        (self.home / '.bash_profile').write_text('export PUB_HOSTED_URL="https://active.example"\n')
        (self.home / '.bash_login').write_text('export PUB_HOSTED_URL="https://inactive.example"\n')
        (self.home / '.profile').write_text('export PUB_HOSTED_URL="https://other.example"\n')
        result = self.run_cli('get', '-en', '-no-color', 'pub')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('PUB_HOSTED_URL=https://active.example\n', result.stdout)

    def test_bash_login_symlink_shares_update_with_legacy_profile(self):
        profile = self.home / '.profile'
        profile.write_text('export PUB_HOSTED_URL=old\n')
        login = self.home / '.bash_profile'
        login.symlink_to(profile)
        result = self.run_cli('set', '-en', '-no-color', 'pub', 'tuna')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertTrue(login.is_symlink())
        self.assertEqual(profile.read_text(),
                         'export PUB_HOSTED_URL="https://mirrors.tuna.tsinghua.edu.cn/dart-pub"\n')
        result = self.run_cli('get', '-en', '-no-color', 'pub')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('PUB_HOSTED_URL=https://mirrors.tuna.tsinghua.edu.cn/dart-pub\n', result.stdout)
        result = self.run_cli('reset', '-en', '-no-color', 'pub')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertTrue(login.is_symlink())
        self.assertEqual(profile.read_text(), '')

    def test_bash_reset_cleans_legacy_login_definitions(self):
        for name in ('.bashrc', '.bash_profile', '.bash_login', '.profile'):
            (self.home / name).write_text('export PUB_HOSTED_URL=old\n')
        result = self.run_cli('reset', '-en', '-no-color', 'pub')
        self.assertEqual(result.returncode, 0, result.stderr)
        for name in ('.bashrc', '.bash_profile', '.bash_login', '.profile'):
            self.assertEqual((self.home / name).read_text(), '')

    @unittest.skipUnless(shutil.which('fish'), 'fish is not installed')
    def test_fish_respects_xdg_config_and_migrates_old_definition(self):
        self.env['SHELL'] = '/usr/bin/fish'
        config = self.home / 'custom config'
        self.env['XDG_CONFIG_HOME'] = str(config)
        fishrc = config / 'fish/config.fish'
        fishrc.parent.mkdir(parents=True)
        fishrc.write_text('# preserve me\nset -gx PUB_HOSTED_URL https://old.example\n')
        result = self.run_cli('set', '-en', '-no-color', 'pub', 'tuna')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(fishrc.read_text(), '# preserve me\n')
        snippet = config / 'fish/conf.d/chsrc.fish'
        self.assertTrue(snippet.is_file())
        self.assertFalse((self.home / '.config').exists())
        actual = subprocess.run(['fish', '-c', 'printenv PUB_HOSTED_URL'], env=self.env,
                                capture_output=True, text=True, timeout=10)
        self.assertEqual(actual.stdout.strip(), 'https://mirrors.tuna.tsinghua.edu.cn/dart-pub')
        before = snippet.read_bytes()
        result = self.run_cli('set', '-en', '-no-color', 'pub', 'tuna')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(snippet.read_bytes(), before)
        viewed = self.run_cli('get', '-en', '-no-color', 'pub')
        self.assertIn('PUB_HOSTED_URL=https://mirrors.tuna.tsinghua.edu.cn/dart-pub', viewed.stdout)
        result = self.run_cli('reset', '-en', '-no-color', 'pub')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(snippet.read_text(), '')

    def test_recipe_roundtrips_in_real_shells(self):
        recipes = [('rustup', 'RUSTUP_DIST_SERVER'), ('pub', 'PUB_HOSTED_URL'),
                   ('flutter', 'FLUTTER_STORAGE_BASE_URL'), ('nvm', 'NVM_NODEJS_ORG_MIRROR'),
                   ('brew', 'HOMEBREW_API_DOMAIN')]
        for shell in ('bash', 'fish'):
            if not shutil.which(shell):
                continue
            self.env['SHELL'] = shutil.which(shell)
            for dish, name in recipes:
                with self.subTest(shell=shell, dish=dish):
                    self.env.pop(name, None)
                    first = self.run_cli('set', '-en', '-no-color', dish, 'tuna')
                    self.assertEqual(first.returncode, 0, first.stderr)
                    path = self.home / ('.bashrc' if shell == 'bash' else '.config/fish/conf.d/chsrc.fish')
                    before = path.read_bytes()
                    again = self.run_cli('set', '-en', '-no-color', dish, 'tuna')
                    self.assertEqual(again.returncode, 0, again.stderr)
                    self.assertEqual(path.read_bytes(), before)
                    viewed = self.run_cli('get', '-en', '-no-color', dish)
                    self.assertEqual(viewed.returncode, 0, viewed.stderr)
                    expected = next(line.split('=', 1)[1] for line in viewed.stdout.splitlines()
                                    if line.startswith(name + '='))
                    self.assertTrue(expected.startswith('https://mirrors.tuna.tsinghua.edu.cn/'))
                    args = (['bash', '--noprofile', '--rcfile', str(path), '-ic', 'printenv ' + name]
                            if shell == 'bash' else ['fish', '-c', 'printenv ' + name])
                    actual = subprocess.run(args, env=self.env, capture_output=True, text=True, timeout=10)
                    self.assertEqual(actual.stdout.strip(), expected)
                    reset = self.run_cli('reset', '-en', '-no-color', dish)
                    self.assertEqual(reset.returncode, 0, reset.stderr)
                    self.assertNotIn(name, path.read_text())

    @unittest.skipUnless(shutil.which('zsh'), 'zsh is not installed')
    def test_zsh_loads_new_profiles_and_zdotdir(self):
        self.env['SHELL'] = shutil.which('zsh')
        for custom in (False, True):
            with self.subTest(custom=custom):
                root = self.home
                if custom:
                    root = self.home / 'zsh configuration'
                    self.env['ZDOTDIR'] = str(root)
                result = self.run_cli('set', '-en', '-no-color', 'pub', 'tuna')
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertTrue((root / '.zshrc').is_file())
                self.assertTrue((root / '.zprofile').is_file())
                actual = subprocess.run(['zsh', '-lic', 'printenv PUB_HOSTED_URL'], env=self.env,
                                        capture_output=True, text=True, timeout=10)
                self.assertEqual(actual.stdout.strip(), 'https://mirrors.tuna.tsinghua.edu.cn/dart-pub')


if __name__ == "__main__":
    unittest.main()
