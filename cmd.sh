ubuntu1804 -c "echo \"#"!"\"/bin/bash$'\n'CUR_CON='$''('ls -1 /dev/pts/ \|wc -l')'$'\n'while  $'\n'DISPLAY=:0 tilix  \&$'\n'sleep 2$'\n'[ '$''('ls -1 /dev/pts/ \|wc -l')' -lt 2 ]$'\n' do true\;$'\n'done$'\n'sleep 10$'\n'while [ '$''('ls -1 /dev/pts/\|wc -l')' -ge 2 ]\; do$'\n'sleep 5$'\n'done|sh "