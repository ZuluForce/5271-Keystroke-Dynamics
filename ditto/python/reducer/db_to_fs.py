import os,sys
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0,parentdir)

import argparse
import reduce_fns as rdc
import db.queries as dbq
from util.json_handler import json_to_object, write_json

def startReduction(outdir):
    records = dbq.getAllCompleted(name="Andrew Helgeson")
    if records is None:
        print("Failed to retrieve records from database")
        return

    print("Processing {} user records from the database".format(len(records)))

    for record in records:
        # Reduce the profile and save it out to the fs
        data = json_to_object(record.data)
        data = rdc.std_reduce(data)

        if record.name is None or record.name == "":
            record.name = "noname"

        out_file = "{}_{}_reduced.json".format(record.name, record.email)
        out_file = os.path.join(outdir, out_file)
        print("name:{} email:{} -> {}".format(record.name, record.email, out_file))


        with open(out_file, "w+") as f:
            write_json(data, fd=f)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Reduce profiles from the database to the filesystem')
    parser.add_argument("-u", "--dbuser", help="username to access the database", default=dbq.db_user)
    parser.add_argument("-p", "--pass", action="store_true", help="will prompt for password if present")
    parser.add_argument("-n", "--dbname", help="name of the db to access", default=dbq.db)
    parser.add_argument("-t", "--table", help="name of the table in the db with the data", default=dbq.db_table)
    parser.add_argument("-m", "--host", "--machine", help="machine hosting the db", default=dbq.db_host)
    parser.add_argument("-o", "--outdir", nargs="?", help="default this directory", default=".")

    args = parser.parse_args()
    dbq.db_user = args.dbuser
    dbq.db = args.dbname
    dbq.db_table = args.table
    dbq.db_host = args.host

    print args

    startReduction(args.outdir)
