HHZZbbAnalysis
=============


To install a complete CMSSW 10X area (including this package)
------------------------------
Please use CMSSW_10_1_6.

Download and execute the setup script:
```
wget -O ${TMPDIR}/checkout_10X.csh https://raw.githubusercontent.com/mkiani/HHZZbb/master/checkout_10X.csh
cd $CMSSW_BASE/src
cmsenv
chmod u+x ${TMPDIR}/checkout_10X.csh
${TMPDIR}/checkout_10X.csh
```

To update this package from the release
------------------------------------------
In the package directory, simply issue
```
git pull
```

To commit and push new changes
------------------------------
To commit directly (you need write access to the repository):
```
git pull
[edit files]
```
Once you are ready to commit
```
git pull
git add [files to be added]
git commit -m ["commit message"] [files to be added]
git push origin master
