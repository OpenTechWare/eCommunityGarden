if [ ! -f serialLog.txt ]; then
    echo "" > serialLog.txt
fi

chown j serialLog.txt
ino serial >> serialLog.txt
