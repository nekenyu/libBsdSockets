#!/bin/sh

echo Updating doc/
rsync -rlptzq --delete doc_built/html/ doc/
# rsync: Do this right or everything is wrong:
# Last line <dir-to-send>/ <parent-to-hold-dir-to-send>/ # Trailing / are REQUIRED

echo Updating doc/ in git to mach as-is
git add -u doc/
