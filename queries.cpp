#include "queries.h"
#include <qsqldatabase.h>
#include <qsqldriver.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
#include "common.h"

namespace queries {

QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
QSqlQuery query(db);

void connectToDatabase()
{
    if (!db.isOpen())
    {
        db.setDatabaseName("database.db");
        db.open();
    }
}

QString genExtQuery(QString entity, QString text)
{
    QString genQuery = "";
    if (text.isEmpty())
    {
        genQuery =  "SELECT DISTINCT " + entity + " FROM ebooks";
    }
    else
    {
        QStringList list = text.split(common::SEP);
        for (QString &ent : list)
        {
            genQuery += "\"" + ent + "\", "; // ent must not be simplified
        }
        genQuery.remove(genQuery.lastIndexOf(','), 1);
    }
    return genQuery;
}

QString genTagQuery(QString tags)
{
    QString genQuery = "";
    if (tags.isEmpty())
    {
        genQuery =  "AND tags LIKE \'%\'";
    }
    else
    {
        QStringList tagList = tags.split(common::SEP);
        for (QString &tag : tagList)
        {
            genQuery += "AND tags LIKE \'%" + tag.simplified() + common::SEP +"%\'";
        }
    }
    return genQuery;
}

QString cleanTags(QString tags)
{
    QString cleanString = "";
    while(tags.endsWith(common::SEP))
    {
        tags.remove(-1, 1); // remove 1 char at pos -1
    }
    QStringList tagList = tags.split(common::SEP);
    for (QString &tag : tagList)
    {
        //cleanString += tag.simplified().remove(',') + ", ";
        cleanString += tag.simplified() + common::SEP;
    }
    return cleanString;
}

// Setup Tables
void createEbooksTable()
{
    query.exec("CREATE TABLE IF NOT EXISTS ebooks ("
                                                    "name TEXT NOT NULL,"
                                                    "author TEXT DEFAULT \'N/A\',"
                                                    "genre TEXT DEFAULT \'N/A\',"
                                                    "path TEXT NOT NULL,"
                                                    "ext TEXT NOT NULL,"
                                                    "pages INT DEFAULT 0,"
                                                    "size INT NOT NULL,"
                                                    "folder TEXT NOT NULL,"
                                                    "tags TEXT DEFAULT \'N/A\' NOT NULL,"
                                                    "summary TEXT DEFAULT \'N/A\' NOT NULL,"
                                                    "time_added DATETIME NOT NULL DEFAULT (datetime(CURRENT_TIMESTAMP, 'localtime')),"
                                                    "PRIMARY KEY (name)"
                                                    ")");

    query.exec("CREATE UNIQUE INDEX IF NOT EXISTS ebooks_names_index ON ebooks(name)");
}

void createSettingsTable()
{
    query.exec("CREATE TABLE IF NOT EXISTS settings");
}

void createSearchTable()
{
    query.exec("CREATE TABLE IF NOT EXISTS searches ( "
                                                    "search_name TEXT NOT NULL, "
                                                    "folder TEXT, "
                                                    "author TEXT, "
                                                    "genre TEXT, "
                                                    "tags TEXT, "
                                                    "ext TEXT, "
                                                    "size_from INT, "
                                                    "size_to INT, "
                                                    "size_in TEXT, "
                                                    "pages_from INT, "
                                                    "pages_to INT, "
                                                    "PRIMARY KEY (search_name) "
                                                    ")");

    query.exec("CREATE INDEX IF NOT EXISTS searches_names_index ON searches(search_name)");

}

void createLinkCollectionsTable()
{
    query.exec("CREATE TABLE IF NOT EXISTS link_collections ( "
                                                    "collection_name TEXT NOT NULL, "
                                                    "PRIMARY KEY (collection_name) "
                                                    ")");
}

void createLinksTable()
{
    query.exec("CREATE TABLE IF NOT EXISTS links ( "
                                                    "link_name TEXT NOT NULL, "
                                                    "link_path TEXT NOT NULL, "
                                                    "collection_id INT NOT NULL, "
                                                    "PRIMARY KEY (link_path, collection_id) "
                                                    ")");
}
// Reset Tables

void resetSettingsTableQuery(){query.exec("DELETE FROM settings");}

void resetEbooksTableQuery(){query.exec("DELETE FROM ebooks");}

void resetSummaries(){query.exec("UPDATE ebooks SET summary=\"N/A\" WHERE summary <> \"N/A\"");}

void resetTags(){query.exec("UPDATE ebooks SET tags=\"N/A" + common::SEP + "\" WHERE tags <> \"N/A" + common::SEP + "\"");}

void resetSearchesTable(){query.exec("DELETE FROM searches");}

// Select
void selectFoldersQuery(){query.exec("SELECT DISTINCT folder FROM ebooks");}

void selectAuthorsQuery(){query.exec("SELECT DISTINCT author FROM ebooks");}

void selectGenreQuery(){query.exec("SELECT DISTINCT genre FROM ebooks");}

void selectSearchesQuery(){query.exec("SELECT search_name FROM searches");}

void selectPath(){query.exec(QString("SELECT path FROM ebooks"));}

void selectExt(){query.exec(QString("SELECT DISTINCT ext FROM ebooks"));}

void selectTags(){query.exec(QString("SELECT DISTINCT tags FROM ebooks"));}

void selectSearchCriteriaQuery(QString searchName)
{
    query.prepare("SELECT * FROM searches WHERE search_name = :searchName");
    query.bindValue(":searchName", searchName);
    query.exec();
}

void selectCountEbooks()
{
    query.prepare(QString("SELECT COUNT(rowid) FROM ebooks"));
    query.exec();
}

void selectNameBasedOnRowid(int rowid)
{
    query.prepare(QString("SELECT name FROM ebooks WHERE rowid = :rowid"));
    query.bindValue(":rowid", rowid);
    query.exec();
}

void selectPathBasedonName(QString name)
{
    query.prepare(QString("SELECT path FROM ebooks WHERE name=:name"));
    query.bindValue(":name", name);
    query.exec();
}

void selectNameBasedOnString(QString stringToSearch){
    query.prepare(QString("SELECT name FROM ebooks WHERE name LIKE :string"));
    query.bindValue(":string", "%" + stringToSearch + "%");
    query.exec();
}

void selectAllBasedonName(QString name)
{
    query.prepare(QString("SELECT * FROM ebooks WHERE name=:name"));
    query.bindValue(":name", name);
    query.exec();
}

void selectSummaryBasedonName(QString name)
{
    query.prepare(QString("SELECT summary FROM ebooks WHERE name=:name"));
    query.bindValue(":name", name);
    query.exec();
}

void selectNameBasedOnCriteria(QString folder, QString genre, QString author, QString tags, QString ext,
                                      long fromPages, long toPages, long long fromSize, long long toSize)
{

    query.prepare(QString("SELECT name FROM ebooks "
                           "WHERE folder in (" + genExtQuery("folder", folder) + ") "
                           + "AND genre in (" + genExtQuery("genre", genre) + ") "
                           + "AND author in (" + genExtQuery("author", author) + ") "
                           + "AND  ((size >= :fromSize AND  size <= :toSize) OR (size IS NULL)) "
                           "AND  ((pages >= :fromPages AND  pages <= :toPages) OR (pages IS NULL)) "
                          "AND ext in (" + genExtQuery("ext", ext) + ") "
                           + genTagQuery(tags) +
                           ""));

    query.bindValue(":fromSize", fromSize);
    query.bindValue(":toSize", toSize);
    query.bindValue(":fromPages", QVariant::fromValue(fromPages));
    query.bindValue(":toPages", QVariant::fromValue(toPages));
    query.exec();
}

int selectCollectionId(QString collectionName)
{
    query.prepare(QString("SELECT rowid FROM link_collections WHERE collection_name = :collection_name"));
    query.bindValue(":collection_name", collectionName);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

void selectCollections(QString searchString)
{
    query.prepare(QString("SELECT collection_name FROM link_collections WHERE collection_name LIKE :string"));
    query.bindValue(":string", "%" + searchString + "%");
    query.exec();
}

void selectLinksBasedOnCollection(QString collectionName, QString searchString)
{
    query.prepare(QString("SELECT l.link_name, l.link_path FROM links l "
                           "INNER JOIN link_collections c ON c.rowid = l.collection_id "
                           "WHERE c.collection_name = :collection "
                           "AND l.link_name LIKE :string"));
    query.bindValue(":collection", collectionName);
    query.bindValue(":string", "%" + searchString + "%");
    query.exec();
}

void selectLinkRecord(QString name)
{
    query.prepare(QString("SELECT link_name, link_path FROM links "
                           "WHERE link_name = :name "));
    query.bindValue(":name", name);
    query.exec();
}
// Insert

void insertBooksQuery(QString name, QString path, QString folder, QString ext, long long size
                             , long pages, QString tags, QString genre, QString author)
{
    query.prepare("INSERT INTO ebooks (name, path, folder, ext, size, pages, tags, genre, author) "
                              "VALUES (:name,:path,:folder,:ext,:size, :pages, :tags, :genre, :author)");
    query.bindValue(":name", name);
    query.bindValue(":path", path);
    query.bindValue(":folder", folder);
    query.bindValue(":ext", ext);
    query.bindValue(":size", size);
    query.bindValue(":pages", QVariant::fromValue(pages));
    tags = cleanTags(tags);
    query.bindValue(":tags", tags);
    query.bindValue(":genre", genre);
    query.bindValue(":author", author);
    query.exec();
}

void insertSearchQuery(QString searchName, QString folder, QString author, QString genre, QString tags,
                              QString ext,
                              int fromSize, int toSize, QString sizeIn, int fromPages, int toPages)
{
    query.prepare(QString("INSERT INTO searches "
                                                   "(search_name, "
                                                    "folder, "
                                                    "author, "
                                                    "genre, "
                                                    "tags, "
                                                    "ext, "
                                                    "size_from, "
                                                    "size_to, "
                                                    "size_in, "
                                                    "pages_from, "
                                                    "pages_to) "
                                                    "VALUES "
                                                    "(:search_name, "
                                                    ":folder, "
                                                    ":author, "
                                                    ":genre, "
                                                    ":tags, "
                                                    ":ext, "
                                                    ":size_from, "
                                                    ":size_to, "
                                                    ":size_in, "
                                                    ":pages_from, "
                                                    ":pages_to) "));

    query.bindValue(":search_name", searchName);
    query.bindValue(":folder", folder);
    query.bindValue(":genre", genre);
    query.bindValue(":author", author);
    query.bindValue(":tags", tags);
    query.bindValue(":ext", ext);
    query.bindValue(":size_from", fromSize);
    query.bindValue(":size_to", toSize);
    query.bindValue(":size_in", sizeIn);
    query.bindValue(":pages_from", fromPages);
    query.bindValue(":pages_to", toPages);

    query.exec();
}

void insertLinkCollection(QString collectionName)
{
    query.prepare(QString("INSERT INTO link_collections (collection_name) "
                                                        "VALUES (:collection_name) "));
    query.bindValue(":collection_name", collectionName);
    query.exec();

}

void insertLink(int collectionId, QString linkName, QString linkPath)
{
    query.prepare(QString("INSERT INTO links (link_name, link_path, collection_id) "
                                            "VALUES (:link_name, :link_path, :collection_id)"));
    query.bindValue(":link_name", linkName);
    query.bindValue(":link_path", linkPath);
    query.bindValue(":collection_id", collectionId);
    query.exec();
}

// Update

void updateBookQuery(QString oldName, QString newName, QString folder, QString genre,
                            QString author, int pages, QString tags, QString path)
{
    query.prepare(QString("UPDATE ebooks "
                          "SET name = :newName, "
                               "folder = :folder, "
                               "genre = :genre, "
                               "author = :author, "
                               "pages = :pages, "
                                "tags = :tags, "
                                "path = :path "
                           "WHERE name = :oldName"));

    query.bindValue(":newName", newName);
    query.bindValue(":folder", folder);
    tags = cleanTags(tags);
    query.bindValue(":tags", tags.simplified());
    query.bindValue(":genre", genre);
    query.bindValue(":author", author);
    query.bindValue(":pages", pages);
    query.bindValue(":path", path);
    query.bindValue(":oldName", oldName);
    query.exec();
}

void updateSummary(QString name, QString summary)
{
    query.prepare("UPDATE ebooks SET summary = :summary WHERE name = :name");
    query.bindValue(":name", name);
    query.bindValue(":summary", summary);
    query.exec();
}

void updateLinkDetails(QString oldName, QString newName, QString path)
{
    query.prepare("UPDATE links SET link_name = :newName, "
                                    "link_path = :path "
                                    "WHERE link_name = :oldName");
    query.bindValue(":newName", newName);
    query.bindValue(":path", path);
    query.bindValue(":oldName", oldName);
    query.exec();
}

void updateLinkCollectionName(QString oldName, QString newName)
{
    query.prepare("UPDATE link_collections SET collection_name = :newName "
                                    "WHERE collection_name = :oldName");
    query.bindValue(":newName", newName);
    query.bindValue(":oldName", oldName);
    query.exec();
}

//Delete
void deleteBook(QString fileName)
{
    query.prepare(QString("DELETE FROM ebooks WHERE name = :name"));
    query.bindValue(":name", fileName);
    query.exec();
}

void deleteLink(QString linkName)
{
    query.prepare(QString("DELETE FROM links WHERE link_name = :name"));
    query.bindValue(":name", linkName);
    query.exec();
}

void deleteCollection(QString collectionName)
{
    query.prepare(QString("SELECT rowid FROM link_collections WHERE collection_name = :name"));
    query.bindValue(":name", collectionName);
    query.exec();

    query.next();
    int collectionId = query.value(0).toInt();

    query.prepare(QString("DELETE FROM links WHERE collection_id = :id"));
    query.bindValue(":id", collectionId);
    query.exec();

    query.prepare(QString("DELETE FROM link_collections WHERE collection_name = :name"));
    query.bindValue(":name", collectionName);
    query.exec();
}
}
