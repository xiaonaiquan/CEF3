@echo off
echo ----------------------------------------------------
echo Press any key to delete all files with ending:
echo 
echo *.aps *.idb *.ncp *.obj *.pch *.sbr *.tmp *.pdb *.bsc *.ilk *.res *.ncb *.opt *.suo *.manifest *.dep
echo 
echo Visual c++/.Net junk 
echo ----------------------------------------------------

pause

del /F /Q /S /A *.ipch *.lastbuildstate *.tlog *.sdf *.aps *.idb *.ncp *.obj *.pch *.sbr *.tmp *.pdb *.bsc *.ilk *.res *.ncb *.opt *.suo *.manifest *.dep

pause





