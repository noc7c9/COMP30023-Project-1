@rsync -avuzR --delete --exclude=*.o --exclude=swap --exclude=rsync2dime.bat --exclude=.git* --exclude=.svn -e ssh . isaleem@dimefox.eng.unimelb.edu.au:project1
