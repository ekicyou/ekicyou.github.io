-- ボトルログ
DROP TABLE dot_bottle_log;
CREATE TABLE dot_bottle_log (
		mid varchar(32) not null,
		logdate timestamp not null,
		channel varchar(40) not null,
		ghost varchar(40) not null,
		sends int2 not null,
		votes int2 not null,
		agrees int2 not null,
		script text not null
    CONSTRAINT PK_dot_bottle_log PRIMARY KEY(mid)
);

CREATE INDEX IX_dot_bottle_log_1 ON item(logdate);
CREATE INDEX IX_dot_bottle_log_2 ON item(channel);
CREATE INDEX IX_dot_bottle_log_3 ON item(ghost);
CREATE INDEX IX_dot_bottle_log_4 ON item(votes);
CREATE INDEX IX_dot_bottle_log_5 ON item(agrees);

-- プラットフォーム
DROP TABLE dot_base
CREATE TABLE dot_base (
    id serial NOT NULL,
		name varchar(32) not null,
		dirbase varchar(32) not null,
		ghost varchar(32) not null,
    CONSTRAINT PK_dot_base PRIMARY KEY(id)
);


-- ユーザー
DROP TABLE dot_user
CREATE TABLE dot_base (
    id serial NOT NULL,
    base_id int2  NOT NULL,

    id serial NOT NULL,
		name varchar(32) not null,
		dirbase varchar(32) not null,
		ghost varchar(32) not null,
    CONSTRAINT PK_dot_base PRIMARY KEY(id)
);







-- キー情報
DROP SEQUENCE SEQ_DOT_KEYS;
CREATE SEQUENCE SEQ_DOT_KEYS;
DROP TABLE DOT_KEYS;
CREATE TABLE DOT_KEYS(
	GHOST_ID	int4 not null,
	ID 				int4 NOT NULL,
	NAME 			varchar(64) not null,

  CONSTRAINT PK_DOT_KEYS PRIMARY KEY(GHOST_ID,id)
);
CREATE UNIQUE INDEX IX_DOT_KEYS_1 ON DOT_KEYS(GHOST_ID,NAME);


-- トークデータ
DROP SEQUENCE seq_dot_talk;
CREATE SEQUENCE seq_dot_talk;
DROP TABLE DOT_TALK;
CREATE TABLE DOT_TALK (
	GHOST_ID	int4 not null,
	ID 				int4 NOT NULL,
	FILENAME 	varchar(64) not null,
	LINE 			int4 not null,
	KEYS 			varchar(100),
	SCRIPT 		text,
	COMMENT 	text,

  CONSTRAINT PK_dot_talk PRIMARY KEY(GHOST_ID,id)
);
CREATE UNIQUE INDEX IX_dot_talk_1 ON DOT_TALK(GHOST_ID,FILENAME,LINE);


-- トーク・キー関連情報
DROP TABLE DOT_TALK_LINK;
CREATE TABLE DOT_TALK_LINK (
	GHOST_ID	int4 not null,
	TALK_ID		int4 NOT NULL,
	KEYS_ID		int4 NOT NULL,

  CONSTRAINT PK_dot_talk_link PRIMARY KEY(GHOST_ID,TALK_ID,KEYS_ID)
);










