(TeX-add-style-hook
 "IAmath"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("article" "12pft" "english")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("natbib" "numbers") ("underscore" "strings") ("geometry" "a4paper" "bindingoffset=0.0in" "left=0.5in" "right=0.5in" "top=1in" "bottom=1in") ("tocbibind" "nottoc" "numbib")))
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "url")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "url")
   (TeX-run-style-hooks
    "latex2e"
    "article"
    "art10"
    "filecontents"
    "natbib"
    "graphicx"
    "url"
    "multicol"
    "underscore"
    "geometry"
    "xcolor"
    "amsfonts"
    "amsmath"
    "tocbibind"))
 :latex)

