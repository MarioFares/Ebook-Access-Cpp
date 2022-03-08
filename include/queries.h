#ifndef QUERIES_H
#define QUERIES_H

#include <QSqlDatabase>
#include <QSqlQuery>

/*
 * All page must be of type quint32 = uint
 * All sizes must be of type quint64 = unsigned long long
 */

namespace queries {

extern QSqlDatabase db;
extern QSqlQuery query;

void connectToDatabase();

QString genExtQuery(QString entity, QString text);

QString genTagQuery(QString tags);

QString cleanTags(QString tags);

// Setup Tables Queries
void setupDb();

void logSessionStart();

void logSessionEnd();

// Reset Tables Queries
void resetSettingsTableQuery();

void resetEbooksTableQuery();

void resetSummaries();

void resetTags();

void resetSearchesTable();

void resetLinksTable();

void resetCollectionsTable();

// Select Queries

void selectEbooksTable ();

void selectLinksTable ();

void selectLinkCollectionsTable ();

void selectSearchesTable ();

void selectSessionLogTable ();

void selectTagsTable ();

void selectAllTable(const QString &tableName);

void selectFoldersQuery();

void selectAuthorsQuery();

void selectGenreQuery();

void selectSearchesQuery();

void selectPath();

void selectExt();

void selectTags();

QString selectTagsBasedOnName(QString name);

void selectSearchCriteriaQuery(QString searchName);

void selectCountEbooks();

void selectNameBasedOnRowid(quint32 rowid);

void selectPathBasedonName(QString name);

void selectNameBasedOnString(QString stringToSearch = "%");

void selectAllBasedonName(QString name);

void selectSummaryBasedonName(QString name);

void selectNameBasedOnCriteria(QString folder, QString genre, QString author, QString tags, QString ext,
                                      quint32 fromPages, quint32 toPages, quint64 fromSize, quint64 toSize);

int selectCollectionId(QString collectionName);

void selectCollections(QString searchString = "%");

void selectLinksBasedOnCollection(QString collectionName, QString searchString);

void selectLinkRecord(QString name);

// Insert Queries
void insertBooksQuery(QString name, QString path, QString folder, QString ext, quint64 size,
                      quint32 pages, QString tags, QString genre, QString author);

void insertSearchQuery(QString searchName, QString folder, QString author, QString genre, QString tags,
                              QString ext, quint32 fromSize, quint32 toSize, QString sizeIn, quint32 fromPages, quint32 toPages);

void insertLinkCollection(QString collectionName);

void insertLink(int collectionId, QString linkName, QString linkPath);

void insertTags(QString tags, QString bookName);
// Update Queries
void updateBookQuery(QString oldName, QString newName, QString folder, QString genre,
                            QString author, quint32 pages, QString tags, QString path);

void updateSummary(QString name, QString summary);

void updateLinkDetails(QString oldName, QString newName, QString path);

void updateLinkCollectionName(QString oldName, QString newName);

//Delete Queries
void deleteBook(QString fileName);

void deleteLink(QString linkName);

void deleteCollection(QString collectionName);
} // Namespace queries
#endif // QUERIES_H
