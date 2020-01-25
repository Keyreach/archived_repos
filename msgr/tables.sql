create table users (
	user_id int not null auto_increment,
	user_name varchar(70) not null,
	user_info varchar(140),
	user_password varchar(70) not null,
	user_type smallint not null,
	primary key(user_id)
);

create table sessions (
	session_id bigint not null,
	session_user int not null,
	primary key(session_id)
);

create table messages (
	msg_id bigint not null auto_increment,
	msg_sender int not null,
	msg_address int not null,
	msg_text text not null,
	primary key(msg_id)
);
