echo "downloading all Input ..."

for dir in $(ls -d */); do
    echo "$(tput setaf 6)=====GETTING INPUT - $(echo $dir | cut -d'/' -f1)=====$(tput setaf 0)"
    if [ ! -d "$dir/input" ]; then
        mkdir $dir/input
        echo "creating input folder"
    fi
    if [ ! -d "$dir/example" ]; then
        mkdir $dir/example
        echo "creating example folder"
    fi
    make -C $dir getInput
done