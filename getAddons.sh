#!/bin/sh
#
#  Created by David Haylock on 01/03/2016.
#

repobase="https://github.com";
repos="julapy/ofxQuadWarp  \
arturoc/ofxHttpUtils \
jefftimesten/ofxJSON  \
braitsch/ofxModal \
braitsch/ofxDatGui \
braitsch/ofxParagraph \
braitsch/ofxSmartFont \
arturoc/ofxTween \
DHaylock/ofxCasioProjectorControl \
kashimAstro/ofxGPIO \
";
echo "|------------------------------|";
echo "|          GET ADDONS          |";
echo "|------------------------------|";
echo "|----> Looking for Addons <----|";

# Print the Directory

#This assumes that you are in the ($(OF_VERSION))/apps/myApps/Stand_and_Stare_Kiosk/

cd ../../../addons

echo "Current Directory $(pwd)";

# Cycle through the Repo's Clone if they don't exist
# If they do then Make sure they are upto date
for repo in ${repos};
    do
        if [ ! -d "${repo}" ]; then
        echo "----> Cloning ${repo} <----";
        git clone "${repobase}/${repo}.git";
        echo "----> Cloned ${repo} <----";
    else
        echo "----> Pulling ${repo} <----";

    # Put us into the addon directory
        cd "${repo}";
        pwd;

        # Pull the latest verison from GitHub
        git pull;

        # Put us one level up
        cd ..;
        echo "----> Pulled ${repo} <----";
    fi
done
exit;
