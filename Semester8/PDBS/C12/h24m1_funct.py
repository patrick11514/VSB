####### h24m1 functions #######

##
from duckdb import DuckDBPyConnection
import sys

def importSomeCSVFiles(con):
  con.execute("copy field_ford FROM 'field_ford.csv'")
  con.execute("copy field_of_science FROM 'field_of_science.csv'")
  # con.execute("copy z_institution FROM 'z_institution.csv' ( HEADER, NULL '');")

  # print(con.sql("select * from z_institution"))
  # print(con.sql("select * from z_field_ford"))
  # print(con.sql("select * from z_field_of_science"))

##
def findFid(fieldFordName, con):
  selectResult = con.sql(f"select fid from Field_ford where name='{fieldFordName}';").fetchone();
  if selectResult is None:
    fid = -1
  else:
    fid = selectResult[0]

  return fid

##
def saveTablesToCsvFiles(con):
  saveTableToCsvFile('recordm1', con)

def saveTableToCsvFile(tableName, con):
  fileName = tableName + '.csv'
  con.execute(f"copy {tableName} TO '{fileName}' (DELIMITER ';');");
  selectResult = con.sql(f"select count(*) from {tableName};").fetchone();
  print('Table ' + tableName + ' exported: ' + str(selectResult[0]) + ' rows')

def createTempTables(con):
  con.execute('''
    CREATE temp TABLE RecordM1Input (
      ico varchar,              -- 0
      institution varchar,      -- 1
      orjk varchar,             -- 2
      institution_unit varchar, -- 3
      recnumber bigint,         -- 4
      type char,                -- 5
      criterion varchar,        -- 6
      author_list varchar,      -- 7
      title varchar,            -- 8
      field_of_science varchar, -- 9 
      field_ford varchar,       -- 10
      period varchar,           -- 11
      degree varchar            -- 12
    );
  ''');

  con.execute('''
    create temp table RecordM1 (
      recid int primary key,
      recnumber char(9) not null,   -- RecordM1Input[4]
      title varchar(1000) not null, -- RecordM1Input[8]
      type char(1) not null,        -- RecordM1Input[5]
      criterion char(1) not null,   -- RecordM1Input[6]: s (Společenská relevance) or p (Přínos k poznání)
      fid int not null,             -- fid of RecordM1Input[10]: references field_ford(fid)
      period char(2) not null,      -- RecordM1Input[11]: 20 (H20), ...
      degree decimal(1,0) not null  -- RecordM1Input[12]
    );
  '''); 
  
  con.execute('''
    create temp table Field_of_science (
      sid int primary key,
      name varchar(50) not null
    );
  ''');

  con.execute('''
    create temp table Field_ford (
      fid int primary key,
      sid int not null,
      name varchar(100) not null
    );
  ''');

  con.execute('''
    create temp table Institution (
      iid int primary key,
      ico char(8) not null,       -- RecordM1Input[0] 
      name varchar(1000) not null -- RecordM1Input[1]
    );
  ''');

  con.execute('''
    create temp table InstitutionUnit (
      uid int primary key,
      iid int,                      -- references intitution(iid)
      orjk varchar(12) not null,    -- RecordM1Input[2] 
      name varchar(1000) not null   -- RecordM1Input[3] 
    );
  ''');

  # con.execute("CREATE INDEX idx_year_field_journal_pk ON year_field_journal(fid, jid, year)")

  con.execute('''
    create temp table Record_InstitutionUnit (
      recid int,
      uid int,
      primary key (recid, uid)
    );
  ''');

#  con.execute('''
#    create temp table author (
#      rid int primary key,
#      name varchar(200) not null,
#      vedidk int
#    );
#  ''');

################################
## Functions to study
################################

def deleteTables(con):
  con.sql("delete from z_article;")
  con.sql("delete from z_author;")
  con.sql("delete from z_journal;")
  con.sql("delete from z_article_author;")
  con.sql("delete from z_institution;")
  con.sql("delete from z_article_institution;")
  con.sql("delete from z_field_ford;")
  con.sql("delete from z_field_of_science;")
  con.sql("delete from z_year_field_journal;")

##
def findAid(article_ut_wos, con):
  selectResult = con.sql(f"select aid from z_article where ut_wos='{article_ut_wos}';").fetchone();
  if selectResult is None:
    aid = -1
  else:
    aid = selectResult[0]

  return aid


##
def insertArticle(aid, jid, ut_wos, name, article_type, year, author_count, con):
  con.execute("insert INTO z_article values (?, ?, ?, ?, ?, ?, ?);",
              (aid, jid, ut_wos, name, article_type, year, author_count));


##
def findJid(name, issn, eissn, czech_or_slovak, jidCnt, rowno, con):
  if not issn and eissn:
    issn = eissn

  if not issn:
    print('empty issn')
 
  selectResult = con.sql(f"select jid from z_journal where name='{name}' or issn='{issn}';").fetchone();

  if selectResult is None:
    jid = jidCnt
    try:
      con.execute(f"insert INTO z_journal values ({jid}, '{name}', '{issn}', '{eissn}', '{czech_or_slovak}');")
      jidCnt = jidCnt + 1
    except:
      print(f'Error (rowno={rowno}): journal has not been inserted: {jid}, {name}, {issn}, {eissn}, {czech_or_slovak}')
      print('jidCnt=' + str(jidCnt))
  else:
    jid = selectResult[0]

  return jid, jidCnt
    
##
def processAuthors(author_list, ridCnt, aid, rowno, con):
  authCntToArticle = 0;
  
  for author_name in author_list.split(';'):
    selectResult = con.sql(f"select rid from z_author where name=?;", params=(author_name,)).fetchone();

    if selectResult is None:
      rid = ridCnt
      con.execute(f"insert INTO z_author values (?, ?, null);", (rid, author_name,))
      ridCnt = ridCnt + 1;
    else:
      rid = selectResult[0]

    # insert z_article_author
    try:
      con.execute(f"insert INTO z_article_author values ({aid}, {rid});")
      authCntToArticle = authCntToArticle + 1
    except:
      print(f'Error (rowno={rowno}): insert into z_article_author values ({aid}, {rid})');

  return authCntToArticle, ridCnt

"""
    create temp table RecordM1 (
      recid int primary key,
      recnumber char(9) not null,   -- RecordM1Input[4]
      title varchar(1000) not null, -- RecordM1Input[8]
      type char(1) not null,        -- RecordM1Input[5]
      criterion char(1) not null,   -- RecordM1Input[6]: s (Společenská relevance) or p (Přínos k poznání)
      fid int not null,             -- fid of RecordM1Input[10]: references field_ford(fid)
      period char(2) not null,      -- RecordM1Input[11]: 20 (H20), ...
      degree decimal(1,0) not null  -- RecordM1Input[12]
    );

"""

def findRecordM1(rowno, recidCnt, recnumber, title, type, criterion, fid, period, degree, con: DuckDBPyConnection):
  selectResult = con.sql(f"select recid from RecordM1 where recnumber='{recnumber}';").fetchone();
  if selectResult is None:
    recid = recidCnt
    try:
      con.execute(f"insert INTO RecordM1 values ({recid}, '{recnumber}', ?, '{type}', '{criterion}', {fid}, '{period}', {degree});", (title,))
      recidCnt = recidCnt + 1
    except Exception as e:
      print(e)
      print(f'Error (rowno={rowno}): record has not been inserted: {recid}, {recnumber}, {title}, {type}, {criterion}, {fid}, {period}, {degree}')
      print('recidCnt=' + str(recidCnt))
      sys.exit()
  else:
    recid = selectResult[0]

  return recid, recidCnt

def findInstitution(iidCnt, ico, institutionName, con):
  selectResult = con.sql(f"select iid from Institution where name='{institutionName}';").fetchone();
  if selectResult is None:
    iid = iidCnt
    try:
      con.execute(f"insert INTO Institution values ({iid}, '{ico}', '{institutionName}');")
      iidCnt = iidCnt + 1
    except Exception as e:
      print(e)
      print(f'Error: institution has not been inserted: {iid}, {institutionName}')
      print('iidCnt=' + str(iidCnt))
      sys.exit()
  else:
    iid = selectResult[0]

  return iid, iidCnt

def findInstitutionUnit(uidCnt, iid, orjk, institutionUnitName, con):
  selectResult = con.sql(f"select uid from InstitutionUnit where iid={iid} and orjk='{orjk}' and name='{institutionUnitName}';").fetchone();
  if selectResult is None:
    uid = uidCnt
    try:
      con.execute(f"insert INTO InstitutionUnit values ({uid}, {iid}, '{orjk}', '{institutionUnitName}');")
      uidCnt = uidCnt + 1
    except Exception as e:
      print(e)
      print(f'Error: institution unit has not been inserted: {uid}, {iid}, {orjk}, {institutionUnitName}')
      print('uidCnt=' + str(uidCnt))
      sys.exit()
  else:
    uid = selectResult[0]

  return uid, uidCnt

def AddInstitutionUnitToRecord(uid, recid, con):
  try:
    con.execute(f"insert INTO Record_InstitutionUnit values ({recid}, {uid});")
  except Exception as e:
    print(e)
    print(f'Error: institution unit has not been added to record: {recid}, {uid}')
    sys.exit()
