
for i in `ls *.eps`; do
  echo "building " ${i%.eps}.png " ... "
  convert $i ${i%.eps}.png
done

