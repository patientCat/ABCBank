/*==============================================================*/
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     2013/8/5 15:56:57                            */
/*==============================================================*/


drop table if exists abstract;

drop table if exists bank_account;

drop table if exists teller;

drop table if exists trans;

/*==============================================================*/
/* table: abstract                                              */
/*==============================================================*/
create table abstract
(
   abstract_id          int not null auto_increment,
   name                 varchar(16),
   primary key (abstract_id)
);

/*==============================================================*/
/* table: bank_account                                          */
/*==============================================================*/
create table bank_account
(
   account_id           int not null auto_increment,
   name                 varchar(16),
   pass                 varchar(16),
   id                   char(18),
   balance              decimal(10,2),
   open_date            datetime,
   flag                 tinyint,
   primary key (account_id)
);

/*==============================================================*/
/* table: teller                                                */
/*==============================================================*/
create table teller
(
   teller_id            int not null auto_increment,
   teller_name          varchar(16),
   teller_pass          varchar(16),
   primary key (teller_id)
);

/*==============================================================*/
/* table: trans                                                 */
/*==============================================================*/
create table trans
(
   trans_id             int not null auto_increment,
   account_id           int,
   other_account_id     int,
   abstract_id          int,
   money                decimal(10,2),
   balance              decimal(10,2),
   trans_date           datetime,
   primary key (trans_id)
);

alter table trans add constraint fk_account_id foreign key (account_id)
      references bank_account (account_id) on delete restrict on update restrict;

alter table trans add constraint fk_abstract_id foreign key (abstract_id)
      references abstract (abstract_id) on delete restrict on update restrict;

alter table trans add constraint fk_other_account_id foreign key (other_account_id)
      references bank_account (account_id) on delete restrict on update restrict;


set names gbk;

/*==============================================================*/
/* ��ʼ����                                                     */
/*==============================================================*/
insert into teller values(1, 'admin', '123456');
insert into abstract values(1, '����');
insert into abstract values(2, '���');
insert into abstract values(3, 'ȡ��');
insert into abstract values(4, 'ת��');
insert into abstract values(5, 'ת��');
