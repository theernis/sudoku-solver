setup.py install
for /R %CD%\build %%f in (*.pyd) do copy %%f %CD%
pause
python main.py
pause