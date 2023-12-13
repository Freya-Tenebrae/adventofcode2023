YEAR="2023"

echo "Execute all..."

for dir in $(ls -d */); do
    echo "$(tput setaf 2)=====DAY  - $(echo $dir | cut -d'/' -f1)=====$(tput setaf 0)"
    echo "$(tput setaf 3)=====PART - 01=====$(tput setaf 0)"
    ./$dir$(echo $YEAR)_$(echo $dir | cut -d'/' -f1)_p1.out ./$(echo $dir)input/input.txt
    echo "$(tput setaf 3)=====PART - 02=====$(tput setaf 0)"
    ./$dir$(echo $YEAR)_$(echo $dir | cut -d'/' -f1)_p2.out ./$(echo $dir)input/input.txt
done