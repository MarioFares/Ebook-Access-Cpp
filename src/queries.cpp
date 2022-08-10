#include "include/common.h"
#include "include/queries.h"

#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QRegularExpression>

namespace queries
{
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
		genQuery = "SELECT DISTINCT " + entity + " FROM ebooks";
	}
	else
	{
		QStringList list = text.split(common::SEP);
		for (QString& ent: list)
		{
			genQuery += "\"" + ent + "\", ";    // ent must not be simplified
		}
		genQuery.remove(genQuery.lastIndexOf(','), 1);
	}

	return genQuery;
}

QString genTagQuery(QString tags)
{
	QString genQuery = "";
	if (!tags.isEmpty())
	{
		genQuery = "AND name in (SELECT ebook FROM ebooks_tags_name WHERE tag in (";
		QStringList tagList = tags.split(common::SEP);
		for (QString& tag: tagList)
		{
			genQuery += "\"" + tag.simplified() + "\", ";;
		}
		genQuery.remove(genQuery.lastIndexOf(','), 1);
		genQuery += "))";
	}

	return genQuery;
}

QString cleanTags(QString tags)
{
	QString cleanString = "";
	while (tags.endsWith(common::SEP))
	{
		tags.remove(-1, 1);    // remove 1 char at pos -1
	}

	QStringList tagList = tags.split(common::SEP);
	for (QString& tag: tagList)
	{
		cleanString += tag.simplified() + common::SEP;
	}

	return cleanString;
}

void setupDb()
{
	QFile file(":/sql/setup.sql");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString sql(file.readAll());
	QStringList statements = sql.split("--break");
	statements.removeAll(QString());
	for (const QString& statement: statements)
	{
		query.exec(statement.simplified());
	}
}

void logSessionStart()
{
	query.exec("INSERT INTO session_log (login) VALUES (Datetime(CURRENT_TIMESTAMP, 'localtime')); ");
}

void logSessionEnd()
{
	query.exec("UPDATE session_log SET logout = (Datetime(CURRENT_TIMESTAMP, 'localtime')) "
			   "WHERE id IN (SELECT id FROM session_log ORDER BY id DESC LIMIT 1); ");
}
// Reset Tables

void resetSettingsTableQuery()
{
	query.exec("DELETE FROM settings");
}

void resetEbooksTableQuery()
{
	query.exec("DELETE FROM ebooks");
}

void resetSummaries()
{
	query.exec(R"(UPDATE ebooks SET summary="N/A "
                "WHERE summary <> "N/A")");
}

void resetTags()
{
	query.exec("DELETE FROM tags");
}

void resetSearchesTable()
{
	query.exec("DELETE FROM searches");
}

void resetLinksTable()
{
	query.exec("DELETE FROM links");
}

void resetCollectionsTable()
{
	query.exec("DELETE FROM link_collections");
}

// Select

// Data Viewer Queries
void selectEbooksTable()
{
	query.exec("SELECT id AS ID,"
			   "name AS Name, "
			   "author AS Author, "
			   "genre AS Genre, "
			   "path AS 'Full Path', "
			   "ext AS Extension, "
			   "pages AS 'Page Count', "
			   "size AS 'Size (bytes)', "
			   "folder AS Folder, "
			   "time_added AS 'Date Added', "
			   "last_modified AS 'Date Modified' "
			   "FROM ebooks");
}

void selectLinksTable()
{
	query.exec("SELECT id AS ID, "
			   "name AS Title, "
			   "path AS Link, "
			   "timestamp AS 'Date Added' "
			   "FROM links");
}

void selectLinkCollectionsTable()
{
	query.exec("SELECT id AS ID, "
			   "name AS 'Collection Name', "
			   "timestamp AS 'Date Added' "
			   "FROM link_collections");
}

void selectSearchesTable()
{
	query.exec("SELECT id AS ID,"
			   "name AS Name,"
			   "folder AS Folder,"
			   "author AS Author, "
			   "genre AS Genre,"
			   "tags AS Tags,"
			   "ext AS Extension,"
			   "size_from 'From Size',"
			   "size_to 'To Size',"
			   "size_in 'Size Unit',"
			   "pages_from 'From Pages', "
			   "pages_to 'To Pages',"
			   "timestamp 'Date Added'"
			   "FROM searches");
}

void selectSessionLogTable()
{
	query.exec("SELECT id AS 'Session ID', "
			   "login AS 'Login Date', "
			   "logout AS 'Logout Time', "
			   "printf(\"%.2f\", duration) AS 'Session Duration (min)' "
			   "FROM session_log");
}

void selectTagsTable()
{
	query.exec("SELECT id AS 'Tag ID', "
			   "name AS 'Tag', "
			   "timestamp AS 'Date Added' "
			   "FROM tags");
}

// End data viewer queries
void selectFoldersQuery()
{
	query.exec("SELECT DISTINCT folder FROM ebooks");
}

void selectAuthorsQuery()
{
	query.exec("SELECT DISTINCT author FROM ebooks");
}

void selectGenreQuery()
{
	query.exec("SELECT DISTINCT genre FROM ebooks");
}

void selectSearchesQuery()
{
	query.exec("SELECT name FROM searches");
}

void selectPath()
{
	query.exec("SELECT path FROM ebooks");
}

void selectExt()
{
	query.exec("SELECT DISTINCT ext FROM ebooks");
}

void selectTags()
{
	query.exec("SELECT name FROM tags");
}

void selectSearchCriteriaQuery(QString searchName)
{
	query.prepare("SELECT * FROM searches WHERE name = :searchName");
	query.bindValue(":searchName", searchName);
	query.exec();
}

void selectCountEbooks()
{
	query.prepare("SELECT COUNT(rowid) FROM ebooks");
	query.exec();
}

void selectNameBasedOnRowid(quint32 rowid)
{
	query.prepare("SELECT name FROM ebooks WHERE rowid = :rowid");
	query.bindValue(":rowid", rowid);
	query.exec();
}

void selectPathBasedonName(QString name)
{
	query.prepare("SELECT path FROM ebooks WHERE name = :name");
	query.bindValue(":name", name);
	query.exec();
}

void selectNameBasedOnString(QString stringToSearch)
{
	query.prepare("SELECT name FROM ebooks WHERE name LIKE :string");
	query.bindValue(":string", "%" + stringToSearch + "%");
	query.exec();
}

QString selectTagsBasedOnName(QString name)
{
	query.prepare("SELECT tag FROM ebooks_tags_name WHERE ebook = :name");
	query.bindValue(":name", name);
	query.exec();

	QStringList tags;
	while (query.next())
	{
		tags.push_back(query.value(0).toString());
	}
	return tags.join(common::SEP);
}

void selectAllBasedonName(QString name)
{
	query.prepare("SELECT * FROM ebooks WHERE name = :name");
	query.bindValue(":name", name);
	query.exec();
}

void selectSummaryBasedonName(QString name)
{
	query.prepare("SELECT summary FROM ebooks WHERE name = :name");
	query.bindValue(":name", name);
	query.exec();
}

void selectNameBasedOnCriteria(QString folder, QString genre, QString author, QString tags, QString ext,
							   quint32 fromPages, quint32 toPages, quint64 fromSize, quint64 toSize)
{
	query.prepare(QString("SELECT name FROM ebooks "
						  "WHERE folder in (" + genExtQuery("folder", folder) + ") " +
			"AND genre in (" + genExtQuery("genre", genre) + ") " +
			"AND author in (" + genExtQuery("author", author) + ") " +
			"AND  ((size >= :fromSize AND  size <= :toSize) OR (size IS NULL)) "
			"AND  ((pages >= :fromPages AND  pages <= :toPages) OR (pages IS NULL)) "
			"AND ext in (" + genExtQuery("ext", ext) + ") " +
			genTagQuery(tags) +
			""));

	query.bindValue(":fromSize", fromSize);
	query.bindValue(":toSize", toSize);
	query.bindValue(":fromPages", QVariant::fromValue(fromPages));
	query.bindValue(":toPages", QVariant::fromValue(toPages));
	query.exec();
}

int selectCollectionId(QString collectionName)
{
	query.prepare("SELECT id FROM link_collections WHERE name = :collection_name");
	query.bindValue(":collection_name", collectionName);
	query.exec();
	query.next();
	return query.value(0).toInt();
}

void selectCollections(QString searchString)
{
	query.prepare("SELECT name FROM link_collections WHERE name LIKE :string");
	query.bindValue(":string", "%" + searchString + "%");
	query.exec();
}

void selectLinksBasedOnCollection(QString collectionName, QString searchString)
{
	query.prepare(QString("SELECT link, path FROM links_collections_name "
						  "WHERE collection = :collection "
						  "AND link LIKE :string"));
	query.bindValue(":collection", collectionName);
	query.bindValue(":string", "%" + searchString + "%");
	query.exec();
}

void selectLinkRecord(QString name)
{
	query.prepare(QString("SELECT name, path FROM links "
						  "WHERE name = :name "));
	query.bindValue(":name", name);
	query.exec();
}

// Insert
void insertBooksQuery(QString name, QString path, QString folder, QString ext, quint64 size,
					  quint32 pages, QString tags, QString genre, QString author, QString publisher, QString published,
					  QString series, quint32 rating, quint32 status)
{
	query.prepare("INSERT INTO ebooks (name, path, folder, ext, size, pages, genre, author, publisher, date_published, series, rating, status) "
				  "VALUES (:name,:path,:folder,:ext,:size, :pages, :genre, :author, :publisher, :published, :series, :rating, :status)");
	query.bindValue(":name", name);
	query.bindValue(":path", path);
	query.bindValue(":folder", folder);
	query.bindValue(":ext", ext);
	query.bindValue(":size", size);
	query.bindValue(":pages", QVariant::fromValue(pages));
	query.bindValue(":genre", genre);
	query.bindValue(":author", author);
	query.bindValue(":publisher", publisher);
	query.bindValue(":published", published);
	query.bindValue(":series", series);
	query.bindValue(":rating", rating);
	query.bindValue(":status", status);
	query.exec();

	insertTags(tags, name);
}

void insertTags(QString tags, QString bookName)
{
	if (tags.isEmpty())
	{
		return;
	}

	for (const QString& tag: cleanTags(tags).split(common::SEP))
	{
		if (tag.isEmpty())
		{
			continue;
		}
		query.prepare("INSERT INTO tags (name) VALUES (:tag_name)");
		query.bindValue(":tag_name", tag);
		query.exec();

		query.prepare("INSERT INTO ebooks_tags_adj (ebook_id, tag_id) VALUES "
					  "((SELECT id FROM ebooks WHERE name = :name), (SELECT id FROM tags WHERE name = :tag))");
		query.bindValue(":name", bookName);
		query.bindValue(":tag", tag);
		query.exec();
	}
}

void insertSearchQuery(QString searchName, QString folder, QString author,
					   QString genre, QString tags, QString ext,
					   quint32 fromSize, quint32 toSize, QString sizeIn,
					   quint32 fromPages, quint32 toPages)
{
	query.prepare(QString("INSERT INTO searches "
						  "(name, "
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
	query.prepare(QString("INSERT INTO link_collections (name) "
						  "VALUES (:collection_name) "));
	query.bindValue(":collection_name", collectionName);
	query.exec();

}

void insertLink(int collectionId, QString linkName, QString linkPath)
{
	query.prepare(QString("INSERT INTO links (name, path) "
						  "VALUES (:link_name, :link_path)"));
	query.bindValue(":link_name", linkName);
	query.bindValue(":link_path", linkPath);
	query.exec();
	query.prepare("INSERT INTO links_collections_adj (collection_id, link_id) "
				  "VALUES (:collection_id, (SELECT id FROM links ORDER BY id DESC LIMIT 1));");
	query.bindValue(":collection_id", collectionId);
	query.exec();
}

// Update
void updateBookQuery(QString oldName, QString newName, QString folder, QString genre,
					 QString author, quint32 pages, QString tags, QString path)
{
	query.prepare(QString("UPDATE ebooks "
						  "SET name = :newName, "
						  "folder = :folder, "
						  "genre = :genre, "
						  "author = :author, "
						  "pages = :pages, "
						  "path = :path "
						  "WHERE name = :oldName"));

	query.bindValue(":newName", newName);
	query.bindValue(":folder", folder);
	query.bindValue(":genre", genre);
	query.bindValue(":author", author);
	query.bindValue(":pages", pages);
	query.bindValue(":path", path);
	query.bindValue(":oldName", oldName);
	query.exec();

	query.prepare("DELETE FROM ebooks_tags_adj WHERE ebook_id IN "
				  "(SELECT id FROM ebooks WHERE name = :newName)");
	query.bindValue(":newName", newName);
	query.exec();

	insertTags(tags, newName);
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
	query.prepare("UPDATE links SET name = :newName, "
				  "path = :path "
				  "WHERE name = :oldName");
	query.bindValue(":newName", newName);
	query.bindValue(":path", path);
	query.bindValue(":oldName", oldName);
	query.exec();
}

void updateLinkCollectionName(QString oldName, QString newName)
{
	query.prepare("UPDATE link_collections SET name = :newName "
				  "WHERE name = :oldName");
	query.bindValue(":newName", newName);
	query.bindValue(":oldName", oldName);
	query.exec();
}

void updateBookName(QString oldName, QString newName)
{
	query.prepare("UPDATE ebooks SET name = :newName WHERE name = :oldName");
	query.bindValue(":newName", newName);
	query.bindValue(":oldName", oldName);
	query.exec();
}

void updateBookPath(QString oldPath, QString newPath)
{
	query.prepare("UPDATE ebooks SET path = :newPath WHERE path = :oldPath");
	query.bindValue(":newPath", newPath);
	query.bindValue(":oldPath", oldPath);
	query.exec();
}

//Delete
void deleteBook(QString fileName)
{
	query.prepare("DELETE FROM ebooks WHERE name = :name");
	query.bindValue(":name", fileName);
	query.exec();
}

void deleteLink(QString linkName)
{
	query.prepare("DELETE FROM links WHERE name = :name");
	query.bindValue(":name", linkName);
	query.exec();
}

void deleteCollection(QString collectionName)
{
	query.prepare("DELETE FROM link_collections WHERE name = :name");
	query.bindValue(":name", collectionName);
	query.exec();
}

void deleteSearch(QString searchName)
{
	query.prepare("DELETE FROM searches WHERE name = :searchName");
	query.bindValue(":searchName", searchName);
	query.exec();
}
}    // Namespace queries
