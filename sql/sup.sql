-- MySQL dump 10.13  Distrib 5.6.20, for Win64 (x86_64)
--
-- Host: localhost    Database: supik
-- ------------------------------------------------------
-- Server version	5.6.20

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `banks`
--

DROP TABLE IF EXISTS `banks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `banks` (
  `idbanks` int(11) NOT NULL,
  `banks` varchar(128) DEFAULT NULL,
  `address` varchar(256) DEFAULT NULL,
  `phone` varchar(45) DEFAULT NULL,
  `www` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idbanks`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `banks`
--

LOCK TABLES `banks` WRITE;
/*!40000 ALTER TABLE `banks` DISABLE KEYS */;
/*!40000 ALTER TABLE `banks` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `chooselists`
--

DROP TABLE IF EXISTS `chooselists`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chooselists` (
  `idchooselists` int(11) NOT NULL,
  `chooselists` varchar(45) DEFAULT NULL,
  `table` varchar(45) DEFAULT NULL,
  `tablefield` varchar(45) DEFAULT NULL,
  `header` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idchooselists`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chooselists`
--

LOCK TABLES `chooselists` WRITE;
/*!40000 ALTER TABLE `chooselists` DISABLE KEYS */;
/*!40000 ALTER TABLE `chooselists` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `countries`
--

DROP TABLE IF EXISTS `countries`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `countries` (
  `idcountries` int(11) NOT NULL,
  `countries` varchar(45) DEFAULT NULL,
  `abbr` varchar(4) DEFAULT NULL,
  PRIMARY KEY (`idcountries`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `countries`
--

LOCK TABLES `countries` WRITE;
/*!40000 ALTER TABLE `countries` DISABLE KEYS */;
/*!40000 ALTER TABLE `countries` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dialogmethods`
--

DROP TABLE IF EXISTS `dialogmethods`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dialogmethods` (
  `iddialogmethods` int(11) NOT NULL,
  `dialogmethods` varchar(45) DEFAULT NULL,
  `dialog` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`iddialogmethods`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dialogmethods`
--

LOCK TABLES `dialogmethods` WRITE;
/*!40000 ALTER TABLE `dialogmethods` DISABLE KEYS */;
INSERT INTO `dialogmethods` VALUES (1,'accept()','ncdialog'),(2,'close()','ncdialog'),(3,'filter()','ncdialog'),(4,'new()','ncdialog'),(5,'refresh()','ncdialog'),(6,'search()','ncdialog');
/*!40000 ALTER TABLE `dialogmethods` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dialogs`
--

DROP TABLE IF EXISTS `dialogs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dialogs` (
  `iddialogs` int(11) NOT NULL,
  `dialogs` varchar(45) DEFAULT NULL,
  `keyfield` varchar(1) DEFAULT NULL,
  `dialogsfields` varchar(45) DEFAULT NULL,
  `dialogslinks` varchar(128) DEFAULT NULL,
  `fieldslocation` varchar(45) DEFAULT NULL,
  `field` varchar(45) DEFAULT NULL,
  `fieldasc` varchar(3) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  `deleted` varchar(45) DEFAULT '0',
  `idpers` varchar(45) DEFAULT NULL,
  `default` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`iddialogs`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dialogs`
--

LOCK TABLES `dialogs` WRITE;
/*!40000 ALTER TABLE `dialogs` DISABLE KEYS */;
INSERT INTO `dialogs` VALUES (1,'whincome','','Приходный ордер','11','30',NULL,'00',NULL,'0',NULL,NULL),(2,'whincome','v','ent.documents.iddocuments','12','31',NULL,'02',NULL,'0',NULL,NULL),(3,'whincome',NULL,'Номер ордера','11','31',NULL,'01',NULL,'0',NULL,NULL),(4,'whincome',NULL,'от','11','31',NULL,'03',NULL,'0',NULL,NULL),(5,'whincome',NULL,'ent.documents.date','2.18.','31',NULL,'04',NULL,'0',NULL,'$date'),(6,'whincome',NULL,'Записать и закрыть','9..this_accept.1.4','35',NULL,'05',NULL,'0',NULL,NULL),(7,'whincome',NULL,'Отменить','9..this_close.2.3','35',NULL,'06',NULL,'0',NULL,NULL),(12,'whincome',NULL,'','13.900','35',NULL,'07',NULL,'0',NULL,NULL),(10,'whincome',NULL,'Поставщик:','11','32.0',NULL,'12',NULL,'0',NULL,NULL),(11,'whincome',NULL,'ent.documents.idsupplier','2.2..ent.contractors.contractors','32.0',NULL,'13',NULL,'0',NULL,NULL),(13,'whincome',NULL,'Автор:','11','35',NULL,'08',NULL,'0',NULL,NULL),(14,'whincome',NULL,'Персонал.ФИО','0','35',NULL,'09',NULL,'0',NULL,'$pers'),(15,'whincome',NULL,'Получатель:','11','32.0',NULL,'14',NULL,'0',NULL,NULL),(16,'whincome',NULL,'ent.documents.idcustomer','2.2..ent.contractors.contractors','32.0',NULL,'15',NULL,'0',NULL,NULL);
/*!40000 ALTER TABLE `dialogs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dirlist`
--

DROP TABLE IF EXISTS `dirlist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dirlist` (
  `iddirlist` int(11) NOT NULL AUTO_INCREMENT,
  `dirlist` varchar(45) DEFAULT NULL,
  `access` varchar(45) DEFAULT NULL,
  `pc` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  `idpers` varchar(45) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`iddirlist`),
  UNIQUE KEY `idtreealiases_UNIQUE` (`iddirlist`)
) ENGINE=MyISAM AUTO_INCREMENT=21 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dirlist`
--

LOCK TABLES `dirlist` WRITE;
/*!40000 ALTER TABLE `dirlist` DISABLE KEYS */;
INSERT INTO `dirlist` VALUES (3,'Номенклатура','01FF','ent.nk',0,NULL,NULL),(4,'Производители','004F','ent.manuf',0,NULL,NULL),(5,'Склады','007F','ent.wh',0,NULL,NULL),(6,'Объекты','004F','ent',1,NULL,NULL),(7,'Персонал','0007','sup.personel',0,NULL,NULL),(8,'Единицы измерения','0007','ent.unitset',0,NULL,NULL),(9,'Категории номенклатуры','004F','ent.classes',0,NULL,NULL),(1,'Наименование справочника','x','pc.',0,NULL,NULL),(2,'Наименование справочника','Права доступа','Ссылка на справочник',0,NULL,NULL),(10,'Контрагенты','1f','ent.contractors',0,NULL,NULL),(11,'Основания','7','ent.reasons',0,NULL,NULL),(12,'Номенклатура','01FF','entent.nk',1,NULL,NULL),(14,'Валюты','7','ent.currency',0,NULL,NULL),(13,'Документы движения','0007','ent.documents',0,NULL,NULL),(15,'Поля ордеров','7','sup.flowfields',0,NULL,NULL),(16,'Периодические','0007','ent.periodic',0,NULL,NULL),(17,'Движения номенклатуры','0007','ent.flow',0,NULL,NULL),(18,'Размещение по складам','F','ent.nkwh',0,NULL,NULL),(19,'Номенклатура (к)','0007','ent.qnk',0,'2001',NULL),(20,'Компоненты Altium (к)','0007','alt.qaltium',0,'2001',NULL);
/*!40000 ALTER TABLE `dirlist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `flowfields`
--

DROP TABLE IF EXISTS `flowfields`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `flowfields` (
  `idflowfields` int(11) NOT NULL,
  `flowfields` varchar(128) DEFAULT NULL,
  `flowheaders` varchar(128) DEFAULT NULL,
  `links` varchar(128) DEFAULT NULL,
  `flowfieldsasc` varchar(128) DEFAULT NULL,
  `date` datetime DEFAULT NULL,
  `deleted` int(1) NOT NULL DEFAULT '0',
  `idpers` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`idflowfields`),
  UNIQUE KEY `idflowfields_UNIQUE` (`idflowfields`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `flowfields`
--

LOCK TABLES `flowfields` WRITE;
/*!40000 ALTER TABLE `flowfields` DISABLE KEYS */;
INSERT INTO `flowfields` VALUES (1,'','Наименование столбца','','Порядок следования полей','0000-00-00 00:00:00',0,NULL),(2,'Имя поля в flow','Наименование столбца','Описание поля','Порядок следования полей','0000-00-00 00:00:00',0,NULL),(3,'-','№','0.4','00','0000-00-00 00:00:00',0,'2001'),(4,'links','Наименование','3.2..ent.qnk.ent.nk','01','0000-00-00 00:00:00',0,'2001'),(5,'quantity','Кол.','6.5..nnnn','02','0000-00-00 00:00:00',0,'2001'),(6,'links','Ед. изм.','4.3..ent.unitset.Номенклатура','03','0000-00-00 00:00:00',0,'2001'),(7,'price','Цена б/НДС','6.4..\"^[0-9]{1,9}[.]{0,1}[0-9]{0,2}$\"','04','0000-00-00 00:00:00',0,'2001'),(8,'-','НДС','5.4..ent.periodic.periodic.НДС.value','07','0000-00-00 00:00:00',0,'2001'),(9,'-','Стоимость','7.4..s.r.c2*c4*c6.c2*c4*c6*c7/100','08','0000-00-00 00:00:00',0,'2001'),(10,'links','Валюта','2.3..ent.currency.abbreviation','05','0000-00-00 00:00:00',0,'2001'),(11,'links','Склад','2.3..ent.wh.wh','09',NULL,0,'2001'),(12,'-','Курс','5.4.5.ent.periodic.periodic.value','06',NULL,0,'2001');
/*!40000 ALTER TABLE `flowfields` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `groups`
--

DROP TABLE IF EXISTS `groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `groups` (
  `idgroups` int(11) NOT NULL,
  `groups` varchar(45) DEFAULT NULL,
  `access` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  PRIMARY KEY (`idgroups`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `groups`
--

LOCK TABLES `groups` WRITE;
/*!40000 ALTER TABLE `groups` DISABLE KEYS */;
INSERT INTO `groups` VALUES (2001,'Администратор','01FF',0),(2002,'Менеджер Altium','00C0',0),(2003,'Менеджер склада','0018',0),(2004,'Управляющий Altium','01C0',0),(2005,'Управляющий склада','0038',0),(2006,'Конструктор','0000',0),(1,'Группа доступа','x',0),(2,'Группа доступа','Доступ по складу',0);
/*!40000 ALTER TABLE `groups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `indexes`
--

DROP TABLE IF EXISTS `indexes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `indexes` (
  `idindexes` int(11) NOT NULL,
  `indexes` varchar(45) DEFAULT NULL,
  `reg` varchar(128) DEFAULT NULL,
  `area` varchar(128) DEFAULT NULL,
  `city` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idindexes`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `indexes`
--

LOCK TABLES `indexes` WRITE;
/*!40000 ALTER TABLE `indexes` DISABLE KEYS */;
/*!40000 ALTER TABLE `indexes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mainmenu`
--

DROP TABLE IF EXISTS `mainmenu`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mainmenu` (
  `idmainmenu` int(11) NOT NULL AUTO_INCREMENT,
  `alias` varchar(45) DEFAULT NULL,
  `idalias` varchar(45) DEFAULT NULL,
  `access` varchar(45) DEFAULT NULL,
  `tooltip` varchar(64) DEFAULT NULL,
  `method` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  `date` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idmainmenu`)
) ENGINE=MyISAM AUTO_INCREMENT=26 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mainmenu`
--

LOCK TABLES `mainmenu` WRITE;
/*!40000 ALTER TABLE `mainmenu` DISABLE KEYS */;
INSERT INTO `mainmenu` VALUES (5,'Сп&равочники','0','1','Справочники','Directories',0,NULL,NULL),(6,'&Компоненты','0','7',NULL,'',0,NULL,NULL),(3,'&Супик','0','1',NULL,'0',0,NULL,NULL),(4,'&Выход','3','1','Выход из программы','ExitSupik',0,NULL,NULL),(8,'Ск&лады','0','1',NULL,'',0,NULL,NULL),(9,'&Документы','0','1','','Directories',0,NULL,NULL),(10,'Сис&администрирование','0','1',NULL,'',0,NULL,NULL),(11,'Уголок &ТБ','0','1',NULL,'',0,NULL,NULL),(12,'&Объекты','0','1',NULL,'',0,NULL,NULL),(13,'С&истема','0','1',NULL,'',0,NULL,NULL),(14,'&Помощь','0','1',NULL,'',0,NULL,NULL),(15,'&Редактирование структуры системы','13','7','Редактор системных элементов - меню, деревьев и т.п.','SysStructEdit',0,NULL,NULL),(7,'&Настройки','13','1','Настройки системы','SettingsEdit',0,NULL,NULL),(1,'Элемент меню','pc.sup.mainmenu','x','','m',0,NULL,NULL),(2,'Наименование элемента меню','Родитель','Права доступа','Подсказка','Вызываемая функция',0,NULL,NULL),(16,'Внимание!','0','FFFF','Есть проблемы!','ProbCheck',0,NULL,NULL),(17,'Принять на склад','8','003F','Приём комплектующих на склад','WhIncome',0,NULL,NULL),(18,'Выдать со склада','8','003F','Списывание комплектующих со склада','WhOutgoing',0,NULL,NULL),(19,'Поиск...','8','7FFF','Поиск комплектующих по складам','WhSearch',0,NULL,NULL),(20,'&Просмотр и редактирование','5','0007','Просмотр и редактирование справочников','Directories',0,NULL,'2001'),(21,'&Экспорт в файл','5','17','Экспорт содержимого справочника в файл','BackupDir',0,NULL,'2001'),(22,'&Импорт из файла','5','17','Импорт справочника из файла','RestoreDir',0,NULL,'2001'),(23,'&Конфигуратор диалогов','13','0007','Конфигуратор диалоговых окон',NULL,0,NULL,'2001'),(24,'Диалог с &2 полями','23','0007','Редактирование диалогов вида Параметр=Значение','DialogEdit',0,NULL,'2001'),(25,'&Диалог общего вида','23','0007','Редактирование диалогов общего вида','DialogEdit',0,NULL,'2001');
/*!40000 ALTER TABLE `mainmenu` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `messages`
--

DROP TABLE IF EXISTS `messages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `messages` (
  `idmessages` int(11) NOT NULL,
  `messages` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `topers` varchar(45) DEFAULT NULL,
  `ackreq` varchar(45) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  `deleted` varchar(45) NOT NULL DEFAULT '0',
  PRIMARY KEY (`idmessages`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `messages`
--

LOCK TABLES `messages` WRITE;
/*!40000 ALTER TABLE `messages` DISABLE KEYS */;
/*!40000 ALTER TABLE `messages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `personel`
--

DROP TABLE IF EXISTS `personel`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `personel` (
  `idpersonel` int(11) NOT NULL AUTO_INCREMENT,
  `personel` varchar(45) DEFAULT NULL,
  `login` varchar(45) DEFAULT NULL,
  `psw` varchar(45) DEFAULT NULL,
  `group` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) DEFAULT '0',
  `date` datetime DEFAULT NULL,
  PRIMARY KEY (`idpersonel`),
  UNIQUE KEY `idpersonel_UNIQUE` (`idpersonel`)
) ENGINE=MyISAM AUTO_INCREMENT=2013 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `personel`
--

LOCK TABLES `personel` WRITE;
/*!40000 ALTER TABLE `personel` DISABLE KEYS */;
INSERT INTO `personel` VALUES (2001,'Валуйских А.О.','vao','evel_489751','2001','0',0,NULL),(2007,'Тарасов А.А.','taa','alan_737478','2002','0',0,NULL),(2008,'Костюк М.В.','kmv','kmv_162886','2002','2001',0,'2014-01-20 13:33:38'),(2009,'Поволяев А.Л.','pal','pal_522068','2002','2001',0,'2014-02-12 15:31:32'),(2010,'Цфасман Г.М.','cgm','grig_835645','2002','2001',0,'2014-02-12 15:32:00'),(2011,'Жуков А.М.','zam','ghu_036819','2001','2001',0,'2014-02-28 16:22:44'),(1,'ФИО',NULL,NULL,'pc.sup.groups','pc.sup.personel',0,'2014-07-21 22:02:00'),(2,'ФИО','Имя ','Пароль','Группа доступа','Создавший',0,'0000-00-00 00:00:00'),(3,'Васин М.Е.','vme','','2003','2001',0,NULL);
/*!40000 ALTER TABLE `personel` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `syslist`
--

DROP TABLE IF EXISTS `syslist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `syslist` (
  `idsyslist` int(11) NOT NULL,
  `alias` varchar(45) NOT NULL,
  `idalias` varchar(45) NOT NULL,
  `access` varchar(45) DEFAULT NULL,
  `pc` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  PRIMARY KEY (`idsyslist`),
  UNIQUE KEY `idsyslist_UNIQUE` (`idsyslist`)
) ENGINE=MyISAM AUTO_INCREMENT=39 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `syslist`
--

LOCK TABLES `syslist` WRITE;
/*!40000 ALTER TABLE `syslist` DISABLE KEYS */;
INSERT INTO `syslist` VALUES (2001,'Работа с БД','0','1',NULL,1),(2002,'Добавить таблицу','2013','1',NULL,1),(2003,'Изменить структуру таблицы','2013','1',NULL,1),(2004,'Создание скелета БД','2001','1',NULL,1),(2005,'Заполнение','2001','1',NULL,1),(2006,'Работа с журналами...','0','1',NULL,1),(2007,'Резервное копирование','0','1',NULL,1),(2008,'Восстановление из резервной копии','2007','1',NULL,1),(2009,'...авторизаций','2006','1',NULL,1),(2010,'Создание резервной копии','2007','1',NULL,1),(2011,'...событий','2006','1',NULL,1),(2012,'Структура разделов номенклатуры','2001','1',NULL,1),(2013,'Таблицы...','2001','1',NULL,1),(2014,'Изменить содержимое таблицы','2013','1',NULL,1),(2015,'Синхронизация с основным сервером','2007','1',NULL,1),(2016,'altium','2014','1',NULL,1),(2017,'enterprise','2014','1',NULL,1),(2018,'Редактирование структуры...','0','1',NULL,0),(2019,'Системные параметры','2018','7','sup.syslist',0),(2020,'Состав справочников','2018','7','sup.dirlist',0),(2023,'Компоненты','0','1',NULL,0),(2022,'Категории','2023','1','altium',0),(2024,'Восстановление удалённых','2001','1',NULL,1),(2026,'Главное меню СУПиКа','2018','7','sup.mainmenu',0),(1,' ','pc.sup.syslist','x','pc.',0),(2,'Наименование пункта меню','Ссылка на родителя','Права доступа','Элементы подменю находятся в...',0),(3,'Состав ордеров','2018','0007','sup.flowfields',0);
/*!40000 ALTER TABLE `syslist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tablefields`
--

DROP TABLE IF EXISTS `tablefields`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tablefields` (
  `idtablefields` int(11) NOT NULL,
  `tablefields` varchar(45) DEFAULT NULL,
  `table` varchar(45) DEFAULT NULL,
  `keyfield` varchar(2) DEFAULT NULL,
  `headers` varchar(128) DEFAULT NULL,
  `links` varchar(128) DEFAULT NULL,
  `fieldsorder` varchar(3) DEFAULT NULL,
  `tablename` varchar(45) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  `deleted` varchar(45) DEFAULT '0',
  `idpers` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idtablefields`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tablefields`
--

LOCK TABLES `tablefields` WRITE;
/*!40000 ALTER TABLE `tablefields` DISABLE KEYS */;
INSERT INTO `tablefields` VALUES (1,'idnk','ent.nk','v','ИД','4.8','00','Номенклатура_полная',NULL,'0',NULL),(2,'nk','ent.nk',NULL,'Наименование','1.8','01','Номенклатура_полная',NULL,'0',NULL),(3,'idclasses','ent.nk',NULL,'Категория','2.2..Категории_сокращ.ИД','02','Номенклатура_полная',NULL,'0',NULL),(4,'idmanuf','ent.nk',NULL,'Производитель','2.2..Производители_сокращ.ИД','03','Номенклатура_полная',NULL,'0',NULL),(5,'idalt','ent.nk',NULL,'Элемент в БД altium','2.11..Категория','04','Номенклатура_полная',NULL,'0',NULL),(6,'idsdw','ent.nk',NULL,'Элемент в БД solidworks','2.11..Категория','05','Номенклатура_полная',NULL,'0',NULL),(7,'idsge','ent.nk',NULL,'Элемент в БД schemagee','2.11..Категория','06','Номенклатура_полная',NULL,'0',NULL),(8,'idctr','ent.nk',NULL,'Элемент в БД конструктивов','2.11..Категория','07','Номенклатура_полная',NULL,'0',NULL),(9,'iddev','ent.nk',NULL,'Элемент в БД устройств','2.11..Категория','08','Номенклатура_полная',NULL,'0',NULL),(10,'idclasses','ent.classes','v','ИД','4.8','00','Категории_сокращ',NULL,'0',NULL),(11,'alias','ent.classes',NULL,'Наименование','2.4..Категории_сокращ','01','Категории_сокращ',NULL,'0',NULL),(12,'idalias','ent.classes',NULL,'ИД_а','2.11..Наименование','02','Категории_сокращ',NULL,'0',NULL),(13,'idmanuf','ent.manuf','v','ИД','4.8','00','Производители_сокращ',NULL,'0',NULL);
/*!40000 ALTER TABLE `tablefields` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-01-16 20:03:23
