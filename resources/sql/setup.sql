PRAGMA foreign_keys = ON;
--break
PRAGMA recursive_triggers = OFF;
--break
CREATE TABLE IF NOT EXISTS ebooks
(
    "id"             INTEGER PRIMARY KEY,
    "name"           TEXT     NOT NULL,
    "author"         TEXT              DEFAULT 'N/A',
    "genre"          TEXT              DEFAULT 'N/A',
    "folder"         TEXT     NOT NULL,
    "series"         TEXT              DEFAULT 'N/A',
    "publisher"      TEXT              DEFAULT 'N/A',
    "date_published" DATE,
    "rating"         INTEGER  NOT NULL DEFAULT 0 CHECK ( rating > - 1 AND rating < 6 ),
    "status"         INTEGER  NOT NULL DEFAULT 0 CHECK ( status > - 1 AND status < 4 ),
    "path"           TEXT     NOT NULL,
    "ext"            TEXT     NOT NULL,
    "pages"          INTEGER           DEFAULT 0,
    "size"           INTEGER  NOT NULL,
    "comments"       TEXT     NOT NULL DEFAULT 'N/A',
    "summary"        TEXT     NOT NULL DEFAULT 'N/A',
    "time_added"     DATETIME NOT NULL DEFAULT (DATETIME(CURRENT_TIMESTAMP, 'localtime')),
    "last_modified"  DATETIME NOT NULL DEFAULT (DATETIME(CURRENT_TIMESTAMP, 'localtime')),
    UNIQUE ("path"),
    UNIQUE ("name")
);
--break
CREATE TABLE IF NOT EXISTS link_collections
(
    "id"        INTEGER PRIMARY KEY,
    "name"      TEXT     NOT NULL COLLATE NOCASE,
    "timestamp" DATETIME NOT NULL DEFAULT (DATETIME(CURRENT_TIMESTAMP, 'localtime')),
    UNIQUE ("name")
);
--break
CREATE TABLE IF NOT EXISTS links
(
    "id"        INTEGER PRIMARY KEY,
    "name"      TEXT     NOT NULL COLLATE NOCASE,
    "path"      TEXT     NOT NULL,
    "comments"  TEXT     NOT NULL,
    "timestamp" DATETIME NOT NULL DEFAULT (DATETIME(CURRENT_TIMESTAMP, 'localtime'))
);
--break
CREATE TABLE IF NOT EXISTS links_collections_adj
(
    "id"            INTEGER PRIMARY KEY,
    "link_id"       TEXT     NOT NULL,
    "collection_id" INTEGER  NOT NULL,
    "timestamp"     DATETIME NOT NULL DEFAULT (DATETIME(CURRENT_TIMESTAMP, 'localtime')),
    FOREIGN KEY ("collection_id") REFERENCES link_collections ("id") ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY ("link_id") REFERENCES links ("id") ON DELETE CASCADE ON UPDATE CASCADE,
    UNIQUE ("link_id", "collection_id")
);
--break
CREATE TABLE IF NOT EXISTS searches
(
    "id"             INTEGER PRIMARY KEY,
    "name"           TEXT     NOT NULL COLLATE NOCASE,
    "folder"         TEXT     NOT NULL,
    "author"         TEXT     NOT NULL,
    "genre"          TEXT     NOT NULL,
    "tags"           TEXT     NOT NULL,
    "ext"            TEXT     NOT NULL,
    "size_from"      INTEGER  NOT NULL,
    "size_to"        INTEGER  NOT NULL,
    "size_in"        TEXT     NOT NULL,
    "pages_from"     INTEGER  NOT NULL,
    "pages_to"       INTEGER  NOT NULL,
    "series"         TEXT     NOT NULL,
    "publisher"      TEXT     NOT NULL,
    "date_published" DATE     NOT NULL,
    "rating"         TEXT     NOT NULL,
    "status"         TEXT     NOT NULL,
    "timestamp"      DATETIME NOT NULL DEFAULT (DATETIME(CURRENT_TIMESTAMP, 'localtime')),
    UNIQUE ("name")
);
--break
CREATE TABLE IF NOT EXISTS tags
(
    "id"        INTEGER PRIMARY KEY,
    "name"      TEXT     NOT NULL COLLATE NOCASE,
    "timestamp" DATETIME NOT NULL DEFAULT (DATETIME(CURRENT_TIMESTAMP, 'localtime')),
    UNIQUE ("name")
);
--break
CREATE TABLE IF NOT EXISTS ebooks_tags_adj
(
    "id"        INTEGER PRIMARY KEY,
    "ebook_id"  INTEGER  NOT NULL,
    "tag_id"    INTEGER  NOT NULL,
    "timestamp" DATETIME NOT NULL DEFAULT (DATETIME(CURRENT_TIMESTAMP, 'localtime')),
    UNIQUE ("ebook_id", "tag_id"),
    FOREIGN KEY ("ebook_id") REFERENCES ebooks ("id") ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY ("tag_id") REFERENCES tags ("id") ON DELETE CASCADE ON UPDATE CASCADE
);
--break
CREATE TABLE IF NOT EXISTS session_log
(
    "id"       INTEGER PRIMARY KEY,
    "login"    DATETIME NOT NULL DEFAULT (DATETIME(CURRENT_TIMESTAMP, 'localtime')),
    "logout"   DATETIME,
    "duration" REAL
);
--break
CREATE TRIGGER IF NOT EXISTS delete_unused_tags
    AFTER DELETE
    ON ebooks_tags_adj
BEGIN
    DELETE FROM tags WHERE NOT EXISTS(SELECT NULL FROM ebooks_tags_adj eta WHERE tags.id = eta.tag_id);
END;
--break
CREATE TRIGGER IF NOT EXISTS delete_unused_links
    AFTER DELETE
    ON links_collections_adj
BEGIN
    DELETE FROM links WHERE NOT EXISTS(SELECT NULL FROM links_collections_adj lca WHERE links.id = lca.link_id);
END;
--break
CREATE TRIGGER IF NOT EXISTS calc_session_dur
    AFTER UPDATE OF logout
    ON session_log
BEGIN
    UPDATE session_log SET duration = (JULIANDAY(logout) - JULIANDAY(login)) * 24 * 60 WHERE id = OLD.id;
END;
--break
CREATE TRIGGER IF NOT EXISTS ebooks_modified
    AFTER UPDATE
    ON ebooks
BEGIN
    UPDATE ebooks SET last_modified = (DATETIME(CURRENT_TIMESTAMP, 'localtime')) WHERE id = OLD.id;
END;
--break
CREATE VIEW IF NOT EXISTS ebooks_tags_name AS
SELECT tags.name AS tag, ebooks.name AS ebook
FROM tags
         INNER JOIN ebooks_tags_adj eta ON eta.tag_id = tags.id
         INNER JOIN ebooks ON eta.ebook_id = ebooks.id;
--break
CREATE VIEW IF NOT EXISTS links_collections_name AS
SELECT links.name AS link, links.path AS path, links.comments AS comments, lc.name AS collection
FROM links
         INNER JOIN links_collections_adj lca ON lca.link_id = links.id
         INNER JOIN link_collections lc ON lca.collection_id = lc.id;
--break
CREATE UNIQUE INDEX IF NOT EXISTS ebooks_index ON ebooks ("name");
--break
CREATE UNIQUE INDEX IF NOT EXISTS searches_index ON searches ("name");
--break
CREATE UNIQUE INDEX IF NOT EXISTS tags_index ON tags ("name");
--break
CREATE UNIQUE INDEX IF NOT EXISTS link_collections_index ON link_collections ("name");
--break
CREATE UNIQUE INDEX IF NOT EXISTS links_index ON links ("name");
