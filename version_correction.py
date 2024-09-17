# Вызывается автоматически из commit.bat для инкремента версии сборки перед коммитом в репозиторий

import os
from datetime import datetime

version_build = -1
version_major = -1
version_minor = -1

def CurrentTime():
    t = datetime.now()
    year = str(t.year)
    return \
        str(t.year) + "-" + \
        '{:-02}'.format(t.month) + "-" + \
        '{:-02}'.format(t.day) + " " + \
        '{:-02}'.format(t.hour) + ":" + \
        '{:-02}'.format(t.minute) + ":" + \
        '{:-02}'.format(t.second)

def WriteVersionToDefines(name_file):
    global version_build
    global version_major
    global version_minor
    lines = []
    with open(name_file, "r", encoding="utf8") as file:
        for line in file:
            lines.append(line)
        file.close()
        with open(name_file, "w", encoding="utf8") as file:
            for line in lines:
                if line.startswith("#define VERSION_BUILD"):
                    strings = line.split(" ")
                    line = strings[0] + " " + strings[1] + " " + str(int(strings[2]) + 1) + "\n"
                    version_build = int(strings[2])
                if line.startswith("#define DATE_BUILD"):
                    strings = line.split(" ")
                    line = strings[0] + " " + strings[1] + " " + "\"" + CurrentTime() + "\"\n"
                if line.startswith("#define VERSION_MAJOR"):
                    strings = line.split(" ");
                    version_major = int(strings[2])
                    print(version_major)
                if line.startswith("#define VERSION_MINOR"):
                    strings = line.split(" ");
                    version_minor = int(strings[2])
                    file.write(line)
            file.close()


# Скорректировать данные в ресурсах
def CorrectResource(name_file):
    lines = []
    with open(name_file, "r", encoding="utf8") as file:
        for line in file:
            lines.append(line)
        file.close()
        string_version = str("\"") + str(version_major) + "." + str(version_minor) + "." + str(version_build) + "." + "0" +"\""
        string_version_2 = str(version_major) + "," + str(version_minor) + "," + str(version_build) + "," + "0"
        print(version_major)
        print(string_version_2)
        with open(name_file, "w", encoding="utf8") as file:
            for line in lines:
                strings = line.strip().split(' ')
                if len(strings) > 2:
                    if strings[1] == "\"FileVersion\",":
                        line = "            VALUE \"FileVersion\", " + string_version + "\n"
                    if strings[1] == "\"ProductVersion\",":
                        line = "            VALUE \"ProductVersion\", " + string_version + "\n"
                if len(strings) == 2:
                    if strings[0] == "FILEVERSION":
                        line = " FILEVERSION " + string_version_2 + "\n"
                    if strings[0] == "PRODUCTVERSION":
                        line = " PRODUCTVERSION " + string_version_2 + "\n"
                file.write(line)
            file.close()

WriteVersionToDefines("sources/VS/Network-PC/defines.h")

CorrectResource("sources/VS/Network-PC/resources/resources.rc")
