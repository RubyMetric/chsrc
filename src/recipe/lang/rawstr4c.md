<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GPL-3.0-or-later
 ! -------------------------------------------------------------
 ! Config Type   : rawstr4c (Markdown)
 ! Config Authors: Aoran Zeng <ccmywish@qq.com>
 ! Contributors  :  Nil Null  <nil@null.org>
 ! Created On    : <2025-07-14>
 ! Last Modified : <2025-07-14>
 ! ---------------------------------------------------------- -->

# rawstr4c input

- prefix = `RAWSTR_pl`
- output = `:global-variable-only-header`
- translate = `:oct`

## Clojar

### project.clj

```clojure
(defproject myapp "1.0.0"
   :description "My Application"
   :dependencies [[lib1 "1.0.0"]
                  [lib2 "2.0.0"]
                  [lib3 "3.0.0"]]
   :mirrors {"clojars" {:name "mirror"
                        :url "@url@"}}
   :main leiningen.web)
```



### projfiles.clj

```clojure
:user {:repositories [["clojars" {:url "@url@"}]]
       ;; other :user profile settings
      }
```
