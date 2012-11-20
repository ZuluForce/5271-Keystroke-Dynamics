import MySQLdb as mdb

"""
Queries for extracting information from the keystroke dynamics collection db
"""

db="5271_project"
db_user="secuser"
db_pass="5271"
db_table="user_data"
db_host="localhost"

con = None

class KDResult:
    def __init__(self):
        self.name = None
        self.email = None
        self.data = None

def getAllCompleted(name=None, email=None):
    global con
    try:
        if con is None:
            con = mdb.connect(db_host,db_user,db_pass,db)

        cur = con.cursor()
        query = "SELECT name,email,json_data FROM {} WHERE submitted=1".format(db_table)
        if name is not None:
            query += " AND name='{}'".format(name)

        if email is not None:
            query += " AND email='{}'".format(email)

        cur.execute(query)

        results = []
        for row in cur.fetchall():
            kd_obj = KDResult()
            kd_obj.name = row[0]
            kd_obj.email = row[1]
            kd_obj.data = row[2]

            results.append(kd_obj)

        return results

    except mdb.Error, e:
        print("Error({}) querying database: {}".format(e.args[0],e.args[1]))
        return None

    finally:
        if con:
            con.close()
            con = None
