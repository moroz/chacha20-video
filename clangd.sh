#!/bin/sh

uname="$(uname | tr -d '\n')"

if [ "$uname" = "Darwin" ]; then
cat <<-EOF > .clangd
CompileFlags:
  Add:
    - "-I$(brew --prefix cunit)/include"
    - "-L$(brew --prefix cunit)/lib"
    - "-lcunit"
EOF
fi
