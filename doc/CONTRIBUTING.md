# 贡献说明

## 分支

- `gh-pipeline`：仅仅在发布版本时由 `@ccmywish` 推送，触发编译到 GitHub Releases 中
- `gh-site`：`chsrc.run` 的工作分支，由 `@ccmywish` 推送
- `main`: stable，代码一定是可以编译运行的，我们假设 end users 在其他条件都得不到二进制时，会自己编译这个分支来运行 `chsrc`
- `dev`：开发分支，工作分支，在此分支上解决冲突

<br>

## 提交与审阅

### 一个简单的 Bug

一个简单的 Bug fix，有写权限的维护者可以直接推送到主仓库的 `dev` 分支

<br>

### 不太容易修复的 Bug 以及新功能

这里要分两种情况考虑。（1）recipe 相关的 （2）framework 相关的

（1）

1. **如果你是 recipe director，则你完全负责这个 recipe，如果你拥有写权限，你可以直接推送代码到 `dev` 分支**
2. 如果你是 recipe maintainer，则你需要参考 [MAINTAINERS.md](./MAINTAINERS.md)，如果只有你一个人，且你拥有写权限，你可以直接推送代码。如果有多人，则需要提一个 issue，介绍方案，然后 @ 所有 maintainer 来 review

---

（2）

1. 需要先搜索你修改的部分涉及到的 recipe，然后提 issue @ 所有相关的 recipe maintainer 来 review
2. 如果涉及了所有 recipe，则 @ framework maintainer，而无需把所有 recipe 的 maintainer 都喊过来，但是如果觉得有必要，可以 @ 任意你觉得有能力 review 和能给出建议的人来 review

<br>

### 最好总是 issue 或 PR

对于有写权限的维护者来说，即使是能够直接推代码，最好也都先提 issue 或 PR，因为这样能够让大家知道代码发生了哪些变动。

如果你觉得要和大家讨论，则 issue；如果你觉得没有讨论的必要了，则直接 PR 后自己立即合并即可。之所以多此一举，是因为这能够显式地记录代码的加入过程，其相当于一份文档方便未来的自己和他人查阅
