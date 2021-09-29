#ifndef QUERIES_H
#define QUERIES_H

#include <qsqldatabase.h>
#include <qsqldriver.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
#include <qmessagebox.h>


using namespace std;

namespace queries {

inline QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
inline QSqlQuery query(db);

inline bool connectToDatabase()
{
    db.setDatabaseName("database.db");
    db.open();
    return db.isOpen();
}

inline QString genExtQuery(QString ext)
{
    QString query = "";
    if (ext.isEmpty())
    {
        query =  "SELECT DISTINCT ext FROM ebooks";
    }
    else
    {
        QStringList tagList = ext.split(",");
        for (QString &tag : tagList)
        {
            query += "\'" + tag.simplified() + "\', ";
        }
        query.remove(query.lastIndexOf(','), 1);
    }
    return query;
}


inline QString genTagQuery(QString tags)
{
    QString query = "";
    if (tags.isEmpty())
    {
        query =  "AND tags LIKE \'%\'";
    }
    else
    {
        QStringList tagList = tags.split(",");
        for (QString &tag : tagList)
        {
            query += "AND tags LIKE \'%" + tag.simplified() + ",%\'";
        }
    }
    return query;
}

inline QString cleanTags(QString tags)
{
    QString cleanString = "";
    while(tags.endsWith(","))
    {
        tags.remove(-1, 1); // remove 1 char at pos -1
    }
    QStringList tagList = tags.split(",");
    for (QString &tag : tagList)
    {
        cleanString += tag.simplified().remove(',') + ", ";
    }
    return cleanString;
}

// Setup Tables
inline void createEbooksTable()
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
                                                    "PRIMARY KEY (name)"
                                                    ")");

    query.exec("CREATE UNIQUE INDEX IF NOT EXISTS ebooks_names_index ON ebooks(name)");
}

inline void createSettingsTable()
{
    query.exec("CREATE TABLE IF NOT EXISTS settings");
}

inline void createSearchTable()
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

inline void createLinkCollectionsTable()
{
    query.exec("CREATE TABLE IF NOT EXISTS link_collections ( "
                                                    "collection_name TEXT NOT NULL, "
                                                    "PRIMARY KEY (collection_name) "
                                                    ")");
}

inline void createLinksTable()
{
    query.exec("CREATE TABLE IF NOT EXISTS links ( "
                                                    "link_name TEXT NOT NULL, "
                                                    "link_path TEXT NOT NULL, "
                                                    "collection_id INT NOT NULL, "
                                                    "PRIMARY KEY (link_path, collection_id) "
                                                    ")");
}
// Reset Tables

inline void resetSettingsTableQuery(){query.exec("DELETE FROM settings");}

inline void resetEbooksTableQuery(){query.exec("DELETE FROM ebooks");}


// Select
inline void selectFoldersQuery(){query.exec("SELECT DISTINCT folder FROM ebooks");}

inline void selectAuthorsQuery(){query.exec("SELECT DISTINCT author FROM ebooks");}

inline void selectGenreQuery(){query.exec("SELECT DISTINCT genre FROM ebooks");}

inline void selectSearchesQuery(){query.exec("SELECT search_name FROM searches");}

inline void selectSearchCriteriaQuery(QString searchName)
{
    query.prepare("SELECT * FROM searches WHERE search_name = :searchName");
    query.bindValue(":searchName", searchName);
    query.exec();
}

inline void selectPath()
{
    query.prepare(QString("SELECT path FROM ebooks"));
    query.exec();
}

inline void selectExt()
{
    query.prepare(QString("SELECT DISTINCT ext FROM ebooks"));
    query.exec();
}

inline void selectTags()
{
    query.prepare(QString("SELECT DISTINCT tags FROM ebooks"));
    query.exec();
}

inline void selectCountEbooks()
{
    query.prepare(QString("SELECT COUNT(rowid) FROM ebooks"));
    query.exec();
}

inline void selectNameBasedOnRowid(int rowid)
{
    query.prepare(QString("SELECT name FROM ebooks WHERE rowid = :rowid"));
    query.bindValue(":rowid", rowid);
    query.exec();
}

inline void selectPathBasedonName(QString name)
{
    query.prepare(QString("SELECT path FROM ebooks WHERE name=:name"));
    query.bindValue(":name", name);
    query.exec();
}

inline void selectNameBasedOnString(QString stringToSearch = "%"){
    query.prepare(QString("SELECT name FROM ebooks WHERE name LIKE :string"));
    query.bindValue(":string", "%" + stringToSearch + "%");
    query.exec();
}

inline void selectAllBasedonName(QString name)
{
    query.prepare(QString("SELECT * FROM ebooks WHERE name=:name"));
    query.bindValue(":name", name);
    query.exec();
}

inline void selectSummaryBasedonName(QString name)
{
    query.prepare(QString("SELECT summary FROM ebooks WHERE name=:name"));
    query.bindValue(":name", name);
    query.exec();
}

inline void selectNameBasedOnCriteria(QString folder, QString genre, QString author, QString tags, QString ext,
                                      long fromPages, long toPages, long long fromSize, long long toSize)
{

    query.prepare(QString("SELECT name FROM ebooks "
                          + QString(folder.isEmpty() ? "WHERE folder LIKE '%' " : "WHERE folder = :folder ")
                           + QString(genre.isEmpty() ? "AND genre LIKE '%' " : "AND genre = :genre ")
                           + QString(author.isEmpty() ? "AND author LIKE '%' " : "AND author = :author ")
                           + "AND  ((size >= :fromSize AND  size <= :toSize) OR (size IS NULL)) "
                           "AND  ((pages >= :fromPages AND  pages <= :toPages) OR (pages IS NULL)) "
                          "AND ext in (" + genExtQuery(ext) + ") "
                           + genTagQuery(tags) +
                           ""));


    query.bindValue(":folder", folder);
    query.bindValue(":genre",   genre);
    query.bindValue(":author",  author);
    query.bindValue(":fromSize", fromSize);
    query.bindValue(":toSize", toSize);
    query.bindValue(":fromPages", QVariant::fromValue(fromPages));
    query.bindValue(":toPages", QVariant::fromValue(toPages));
    query.exec();
}

inline int selectCollectionId(QString collectionName)
{
    query.prepare(QString("SELECT rowid FROM link_collections WHERE collection_name = :collection_name"));
    query.bindValue(":collection_name", collectionName);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

inline void selectCollections(QString searchString = "%")
{
    query.prepare(QString("SELECT collection_name FROM link_collections WHERE collection_name LIKE :string"));
    query.bindValue(":string", "%" + searchString + "%");
    query.exec();
}

inline void selectLinksBasedOnCollection(QString collectionName, QString searchString)
{
    query.prepare(QString("SELECT l.link_name FROM links l "
                           "INNER JOIN link_collections c ON c.rowid = l.collection_id "
                           "WHERE c.collection_name = :collection "
                           "AND l.link_name LIKE :string"));
    query.bindValue(":collection", collectionName);
    query.bindValue(":string", "%" + searchString + "%");
    query.exec();
}

inline void selectLinkRecord(QString name)
{
    query.prepare(QString("SELECT link_name, link_path FROM links "
                           "WHERE link_name = :name "));
    query.bindValue(":name", name);
    query.exec();
}
// Insert

inline void insertBooksQuery(QString name, QString path, QString folder, QString ext, long long size
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

inline void insertSearchQuery(QString searchName, QString folder, QString author, QString genre, QString tags,
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

inline void insertLinkCollection(QString collectionName)
{
    query.prepare(QString("INSERT INTO link_collections (collection_name) "
                                                        "VALUES (:collection_name) "));
    query.bindValue(":collection_name", collectionName);
    query.exec();

}

inline void insertLink(int collectionId, QString linkName, QString linkPath)
{
    query.prepare(QString("INSERT INTO links (link_name, link_path, collection_id) "
                                            "VALUES (:link_name, :link_path, :collection_id)"));
    query.bindValue(":link_name", linkName);
    query.bindValue(":link_path", linkPath);
    query.bindValue(":collection_id", collectionId);
    query.exec();
}

// Update

inline void updateBookQuery(QString oldName, QString newName, QString folder, QString genre,
                            QString author, int pages, QString tags)
{
    query.prepare(QString("UPDATE ebooks "
                          "SET name = :newName, "
                               "folder = :folder, "
                               "genre = :genre, "
                               "author = :author, "
                               "pages = :pages, "
                                "tags = :tags "
                           "WHERE name = :oldName"));

    query.bindValue(":newName", newName);
    query.bindValue(":folder", folder);
    tags = cleanTags(tags);
    query.bindValue(":tags", tags.simplified());
    query.bindValue(":genre", genre);
    query.bindValue(":author", author);
    query.bindValue(":pages", pages);
    query.bindValue(":oldName", oldName);
    query.exec();
}

inline void updateSummary(QString name, QString summary)
{
    query.prepare("UPDATE ebooks SET summary = :summary WHERE name = :name");
    query.bindValue(":name", name);
    query.bindValue(":summary", summary);
    query.exec();
}

inline void updateLinkDetails(QString oldName, QString newName, QString path)
{
    query.prepare("UPDATE links SET link_name = :newName, "
                                    "link_path = :path "
                                    "WHERE link_name = :oldName");
    query.bindValue(":newName", newName);
    query.bindValue(":path", path);
    query.bindValue(":oldName", oldName);
    query.exec();
}

inline void updateLinkCollectionName(QString oldName, QString newName)
{
    query.prepare("UPDATE link_collections SET collection_name = :newName "
                                    "WHERE collection_name = :oldName");
    query.bindValue(":newName", newName);
    query.bindValue(":oldName", oldName);
    query.exec();
}

//Delete
inline void deleteBook(QString fileName)
{
    query.prepare(QString("DELETE FROM ebooks WHERE name = :name"));
    query.bindValue(":name", fileName);
    query.exec();
}

inline void deleteLink(QString linkName)
{
    query.prepare(QString("DELETE FROM links WHERE link_name = :name"));
    query.bindValue(":name", linkName);
    query.exec();
}

inline void deleteCollection(QString collectionName)
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
#endif // QUERIES_H
