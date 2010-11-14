
List help files to install

for i in `find ./eng/ ./cat/ ./esp/ -type f | grep -v .zip$`; do echo -n $i " "; done
