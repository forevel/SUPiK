#!/usr/local/bin/bash
#set -x

# mysql server hostname
host='localhost'

# mysql data base user
db_user='supik'

# mysql data base password
db_passwd='sysupik'

# mysql data base name
db_names=(altium devices constructives enterprise supik schemagee solidworks sysadm tb)

# mysql data base_dump name
db_dump_name=(alt.sql dev.sql ctr.sql ent.sql sup.sql sch.sql sol.sql sys.sql tb.sql)

# mysql port
port='3306'

#######################################################################################################
#clear

#check dump exists
for i in 0 1 2 3 4 5 6 7 8
do
	if [ ! -f "${db_dump_name[i]}" ]; then
	  echo "The file '${db_dump_name[i]}' does not exist."
	  echo "The dump should be saved within the same directory as the script."
	  exit
	fi
done

#Data base root-user who is allowed to create the data base (e.g. root)
echo "Enter a user having rights to create a new"
echo -n "user as well as a new data base: "
read root_user
echo ""  # force a carriage return to be output
echo ""
echo ""

#Data base root-user password
echo -n "Enter root user password '$root_user': "
stty -echo
read root_passwd
stty echo
echo ""  # force a carriage return to be output
echo ""
echo ""


if [ "$db_passwd" = "" ]; then
  #Data base user password
  echo -n "Enter the data base user's password '$db_user': "
  stty -echo
  read db_passwd
  stty echo
  echo ""  # force a carriage return to be output
  if [ "$db_passwd" = "" ]; then
    echo "Password mustn't be empty."
    exit
  fi
fi

echo ""
echo ""
echo "A new user will be created having the user name '$db_user'"
echo "and a new databases will be created."
echo "Please wait..."
echo ""
mysql --port=$port --user=$root_user --password=$root_passwd << EOFMYSQL
  #Create database user
  GRANT USAGE ON *.* TO '$db_user'@'%' IDENTIFIED BY '$db_passwd';
EOFMYSQL

for i in 0 1 2 3 4 5 6 7 8
do
	mysql --port=$port --user=$root_user --password=$root_passwd << EOFMYSQL2
	CREATE DATABASE ${db_names[i]} CHARACTER SET utf8 COLLATE utf8_general_ci;
	GRANT SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, INDEX, ALTER, CREATE TEMPORARY TABLES, EXECUTE, CREATE ROUTINE, ALTER ROUTINE, TRIGGER ON ${db_names[i]}.* TO '$db_user'@'%';
EOFMYSQL2
	# $db_name < $db_dump_name
	mysql --port=$port --user=$root_user --password=$root_passwd ${db_names[i]} < ${db_dump_name[i]}
done

echo -n "done!"
exit 0
