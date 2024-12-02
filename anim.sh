#!/bin/bash

# Input scene file
INPUT_SCENE="scenes/columns.rt"
# Temporary scene file for modifications
TEMP_SCENE=$(mktemp)
# Starting and ending X coordinates
START_X=-10
END_X=10
# Step size for X coordinate
STEP=0.1
# Cleanup function to remove temporary files
cleanup() {
    rm -f "$TEMP_SCENE"
}
trap cleanup EXIT
# Counter for frame numbering
frame=0
# Iterate from START_X to END_X
current_x=$START_X
while (( $(echo "$current_x <= $END_X" | bc -l) )); do
    # Create a copy of the original scene file
    cp "$INPUT_SCENE" "$TEMP_SCENE"

    # Adjust only the line with sp -10,5,0
    sed -i -E "s/^sp\s*-10,5,0(.*)/sp $current_x,5,0\1/" "$TEMP_SCENE"

    # Render the scene
    ./miniRT "$TEMP_SCENE"

    # Rename output.png to frame[X].png
    mv output.png "frame_$(printf "%03d" $frame).png"

    # Increment variables
    frame=$((frame + 1))
    current_x=$(echo "$current_x + $STEP" | bc)
done

echo "Rendered $frame frames from X=$START_X to X=$END_X"
