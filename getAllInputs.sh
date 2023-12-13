echo "downloading all Input ..."

for dir in $(ls -d */); do
    echo "$(tput setaf 6)=====GETTING INPUT - $(echo $dir | cut -d'/' -f1)=====$(tput setaf 0)"
    make -C $dir getInput
done