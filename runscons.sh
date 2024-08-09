#TODO: kill all old bash terminals

# Create a temporary file to store PID and 5th column
tmpfile=$(mktemp)

# Extract PID and 5th column, store in temporary file
tasklist | grep -i "Godot" | awk '{print $2, $5}' > "$tmpfile"

# Sort the temporary file based on the 5th column (numeric sort)
sort -nr "$tmpfile" > "$tmpfile.sorted"

# Extract sorted PIDs
pids=$(cut -d' ' -f1 "$tmpfile.sorted")

# Remove temporary files
rm "$tmpfile" "$tmpfile.sorted"

# Check if two PIDs found
num_pids=$(echo "$pids" | wc -w)
if [ "$num_pids" -eq 2 ]; then
#   Kill each process
  for pid in $pids
  do
    echo "Killing $pid..."
    taskkill //PID $pid
    break
  done
else
  echo "Found $num_pids Godot process. Expected 2. No taskkill for game."
fi

cd ..

#Run scons
scons platform=windows

timeout=6000
echo "Process finished. Terminal will close in $timeout seconds."
sleep $timeout