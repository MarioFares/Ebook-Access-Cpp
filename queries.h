#ifndef QUERIES_H
#define QUERIES_H

#include <qsqldatabase.h>
#include <qsqlquery.h>

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
void createEbooksTable();

void createSettingsTable();

void createSearchTable();

void createLinkCollectionsTable();

void createLinksTable();

// Reset Tables Queries
void resetSettingsTableQuery();

void resetEbooksTableQuery();

void resetSummaries();

void resetTags();

void resetSearchesTable();


// Select Queries
void selectFoldersQuery();

void selectAuthorsQuery();

void selectGenreQuery();

void selectSearchesQuery();

void selectPath();

void selectExt();

void selectTags();

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
