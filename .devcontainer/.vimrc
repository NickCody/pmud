set encoding=utf-8
set t_Co=256
set fillchars+=stl:\ ,stlnc:\
set term=xterm-256color
set termencoding=utf-8
let g:airline_powerline_fonts=1
set fileencodings=utf-8
set encoding=utf-8
"set diffopt+=iwhite
set expandtab
set tabstop=4

if exists('$TMUX')
  let &t_SI = "\<Esc>Ptmux;\<Esc>\<Esc>]50;CursorShape=1\x7\<Esc>\\"
  let &t_EI = "\<Esc>Ptmux;\<Esc>\<Esc>]50;CursorShape=0\x7\<Esc>\\"
else
  let &t_SI = "\<Esc>]50;CursorShape=1\x7"
  let &t_EI = "\<Esc>]50;CursorShape=0\x7"
end

map \\t <Esc>:set noexpandtab tabstop=2 shiftwidth=2<CR>
map \\T <Esc>:set noexpandtab tabstop=4 shiftwidth=4<CR>
map \t <Esc>:set expandtab tabstop=2 shiftwidth=2<CR>
map \T <Esc>:set expandtab tabstop=4 shiftwidth=4<CR>
