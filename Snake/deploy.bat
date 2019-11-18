call C:\Users\Victor\Anaconda3\Scripts\activate.bat
call conda activate GYM_ENV_RL

mkdir moc_files

C:\Qt\5.9.7\msvc2017_64\bin\moc.exe snakegame.h -o moc_files/snakegame_moc.cpp
C:\Qt\5.9.7\msvc2017_64\bin\moc.exe widget.h -o moc_files/widget_moc.cpp

pip install .
pause