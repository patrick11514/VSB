import duckdb
import h24m1_funct
import sys
from pathlib import Path

con = duckdb.connect('h24m1.db')


h24m1_funct.createTempTables(con)

con.execute("delete from RecordM1Input;");

inputFile = 'm1_records.csv';

try:
  con.execute(f"copy RecordM1Input from { inputFile }");
except Exception as e:
  print(f"Error: file has not been imported: { inputFile }")
  print(e)
  sys.exit()

print(f"Start to process file: { inputFile }")

result = con.sql("select * from RecordM1Input").fetchall()
print("#Rows:", len(result))

h24m1_funct.importSomeCSVFiles(con)

# counters to compute recid, iid, uid
recidCnt = 0;
iidCnt = 0;
uidCnt = 0;

## import individual rows
for rowno, row in enumerate(result):
  fid = h24m1_funct.findFid(row[10], con);

  institutionUnitName = row[1]
  orjk = row[2]
  recnumber = row[4]
  title = row[8]
  type = row[5]
  criterion = row[6]
  period = row[11]
  degree = row[12]

  if not degree.isdigit() or orjk is None:
    continue

  ico = row[0]

  recid, recidCnt = h24m1_funct.findRecordM1(rowno, recidCnt, recnumber, title, type, criterion, fid, period, degree, con)
  iid, iidCnt = h24m1_funct.findInstitution(iidCnt, ico, institutionUnitName, con)
  uid, uidCnt = h24m1_funct.findInstitutionUnit(uidCnt, iid, orjk, institutionUnitName, con)
  h24m1_funct.AddInstitutionUnitToRecord(recid, uid, con)
  print(f"\rRows processed: {rowno}", end="\r")

h24m1_funct.saveTablesToCsvFiles(con)

print(f"recidCnt={recidCnt}, iidCnt={iidCnt}, uidCnt={uidCnt}");

con.close();
