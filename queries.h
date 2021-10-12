#ifndef QUERIES_H
#define QUERIES_H

#include <qsqldatabase.h>
#include <qsqlquery.h>

using namespace std;

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

void selectNameBasedOnRowid(int rowid);

void selectPathBasedonName(QString name);

void selectNameBasedOnString(QString stringToSearch = "%");

void selectAllBasedonName(QString name);

void selectSummaryBasedonName(QString name);

void selectNameBasedOnCriteria(QString folder, QString genre, QString author, QString tags, QString ext,
                                      long fromPages, long toPages, long long fromSize, long long toSize);

int selectCollectionId(QString collectionName);

void selectCollections(QString searchString = "%");

void selectLinksBasedOnCollection(QString collectionName, QString searchString);

void selectLinkRecord(QString name);

// Insert Queries
void insertBooksQuery(QString name, QString path, QString folder, QString ext, long long size,
                      long pages, QString tags, QString genre, QString author);

void insertSearchQuery(QString searchName, QString folder, QString author, QString genre, QString tags,
                              QString ext, int fromSize, int toSize, QString sizeIn, int fromPages, int toPages);

void insertLinkCollection(QString collectionName);

void insertLink(int collectionId, QString linkName, QString linkPath);

// Update Queries
void updateBookQuery(QString oldName, QString newName, QString folder, QString genre,
                            QString author, int pages, QString tags, QString path);

void updateSummary(QString name, QString summary);

void updateLinkDetails(QString oldName, QString newName, QString path);

void updateLinkCollectionName(QString oldName, QString newName);

//Delete Queries
void deleteBook(QString fileName);

void deleteLink(QString linkName);

void deleteCollection(QString collectionName);
}
#endif // QUERIES_H
