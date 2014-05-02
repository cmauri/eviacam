#/bin/sh

if [ $# -lt 1 ]; then
	echo "Not enough parameters"
	echo "Usage: $0 <version>"
	echo "Example: $0 1.6.0-1ubuntu1"
	exit -1
fi

if [ ! -f ubuntu-releases ]; then
	echo "File ubuntu-releases not found"
	echo "Run from inside ubuntu directory"
	exit -1
fi

cd ..

if [ -f debian-src/eviacam*orig.tar.gz ]; then
	echo "Source file detected. Clean debian-src/ directory first"
	exit -1
fi

echo "This script overwrites debian/rules, debian/control and debian/changelog"
echo "Press ENTER to continue..."
read

basedir=ubuntu
version=$1
changelogf="debian/changelog"

unset LANG
for release in `cat $basedir/ubuntu-releases`; do
	echo -n "Processing $release"
	# Create symlinks to rules files
	ln -s rules $basedir/rules-$release 2> /dev/null
	cat $basedir/rules-$release > debian/rules
	
	# Create symlinks to control files
	ln -s control $basedir/control-$release 2> /dev/null
	cat $basedir/control-$release > debian/control
	
	# Create changelog
	echo "eviacam ($version~$release) $release; urgency=low" > $changelogf
	echo "" >> $changelogf
	echo "  * Initial release" >> $changelogf
	echo "" >> $changelogf
	echo -n " -- Cesar Mauri <cesar@crea-si.com>  " >> $changelogf
	date -R >> $changelogf
	echo "" >> $changelogf

	if [ ! -f debian-src/eviacam*orig.tar.gz ]; then
		# Source package does not exists. Create
		make deb-src
	else
		make deb-src-changes
	fi
	
	echo "Processing $release: done."
done
