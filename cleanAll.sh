echo "Cleaning all..."

for dir in $(ls -d */); do
    echo "$(tput setaf 6)=====CLEAN - $(echo $dir | cut -d'/' -f1)=====$(tput setaf 0)"
    make -C $dir fclean
done