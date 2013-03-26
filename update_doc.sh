#!/bin/sh

# Update doc/ directory in this branch from .gitignored documentation built on
# master (or any branch)
#
# Setup
# 1) In branch you want to build documentation in (i.e. master):
#   a) build directory configured in SRC variable in this script (below)
#   b) ensure directory is in .gitignore in that branch
#
# 2) In branch you want documentation in (i.e. gh-pages):
#  a) add this script
#  b) configure SRC and DEST variables (below)
#
# Usage:
# 1) In branch you want to build documentation in (i.e. master):
#  a) build documentation
#  b) commit your work (not including the documentation)
# 
# 2) Switch branches to the branch you want documentation in (i.e. gh-pages):
#  a) git checkout <branch>
#
# 3) In branch you want documentation in (i.e. gh-pages):
#  a) run this script (i.e. ./update-doc.sh)
#  b) git commit

# Directory to copy from
# 
# Should be .gitignored in the branch it is built in
#
# NOTE: have trailing/ if you want directory _contents_ pushed
SRC=doc_built/html/

# Destination directory
#
# Directory in current branch to copy to
#
# Note: Trailing / required
DEST=doc/

echo Updating ${DEST} from ${SRC}:

rsync -rlptzq --delete ${SRC} ${DEST}
# rsync: Do this right or everything is wrong:
# Last line <dir-to-send>/ <parent-to-hold-dir-to-send>/ # Trailing / are REQUIRED

echo Updating ${DEST} with 'git add -u':

git add -u doc/
