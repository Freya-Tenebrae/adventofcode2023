echo "Execute all..."

for dir in $(ls -d */); do
    echo "$(tput setaf 5)=====DAY  - $(echo $dir | cut -d'/' -f1)=====$(tput setaf 0)"
    make -C $dir
done