-- MySQL dump 10.13  Distrib 5.6.22, for Win64 (x86_64)
--
-- Host: localhost    Database: enterprise
-- ------------------------------------------------------
-- Server version	5.6.22

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
-- Table structure for table `classes`
--

DROP TABLE IF EXISTS `classes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `classes` (
  `idclasses` int(11) NOT NULL,
  `alias` varchar(45) NOT NULL,
  `idalias` varchar(45) DEFAULT NULL,
  `parameters` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  `idpers` varchar(128) DEFAULT NULL,
  `date` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`idclasses`)
) ENGINE=MyISAM AUTO_INCREMENT=901 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `classes`
--

LOCK TABLES `classes` WRITE;
/*!40000 ALTER TABLE `classes` DISABLE KEYS */;
INSERT INTO `classes` VALUES (2001,'Конденсаторы','0','3',0,'2001',NULL),(2050,'Диоды','0','5',0,NULL,NULL),(2080,'Транзисторы','0','12',0,'2001',NULL),(2110,'Резисторы','0','11',0,'2001',NULL),(2111,'Постоянные','2110',NULL,1,NULL,NULL),(2112,'Резисторы','2111',NULL,1,NULL,NULL),(2113,'Чип-резисторы','2112',NULL,0,NULL,NULL),(2156,'Выводные резисторы','2112',NULL,0,NULL,NULL),(2199,'Резисторные сборки','2111',NULL,1,NULL,NULL),(2200,'Чип-резисторные сборки','2199',NULL,0,NULL,NULL),(2207,'Выводные резисторные сборки','2199',NULL,0,NULL,NULL),(2210,'Переменные резисторы','2110',NULL,1,NULL,NULL),(2211,'Подстроечные резисторы','2210',NULL,1,NULL,NULL),(2216,'Специальные резисторы','2110',NULL,1,NULL,NULL),(2217,'Резисторы прочие','2110',NULL,0,NULL,NULL),(2218,'Переменные резисторы','2210',NULL,1,NULL,NULL),(2300,'Разъёмы','0','4',0,NULL,NULL),(39,'Генераторы, кварцы','0','28',0,'2001',NULL),(2400,'Микросхемы','0',NULL,0,NULL,NULL),(2500,'Механические компоненты','0',NULL,1,NULL,NULL),(2600,'Источники питания','0','10',0,'2001',NULL),(2650,'Защитные диоды','0','21',0,'2001',NULL),(2700,'Прочее','0',NULL,0,NULL,NULL),(2800,'Изделия','0',NULL,0,NULL,NULL),(2900,'Конструктивы','0',NULL,1,NULL,NULL),(2002,'Постоянные','2001',NULL,1,NULL,NULL),(2003,'Переменные','2001','3',0,'2001',NULL),(2004,'Специальные','2001',NULL,1,NULL,NULL),(2005,'Прочие','2001','3',0,'2001',NULL),(2006,'Чип','2002',NULL,1,NULL,NULL),(2007,'Выводные','2002',NULL,1,NULL,NULL),(2008,'Электролитические','2006',NULL,1,NULL,NULL),(2009,'Керамические','2006',NULL,0,NULL,NULL),(2010,'Пленочные','2006',NULL,1,NULL,NULL),(2011,'Электролитические','2007',NULL,0,NULL,NULL),(2012,'Керамические','2007',NULL,0,NULL,NULL),(2041,'Трансформаторы','0','',0,'2001',NULL),(2042,'Устройства коммутации','0','16',0,'2001',NULL),(2043,'Переключатели','2042','16',0,'2001',NULL),(2044,'Тактовые кнопки','2042','16',0,'2001',NULL),(2045,'Варисторы','0','',0,'2001',NULL),(2046,'Варисторы smd','2045','20',0,'2001',NULL),(2047,'Варисторы выводные','2045','20',0,'2001',NULL),(2048,'Разрядники','0','',0,'2001',NULL),(2049,'Чип','2048',NULL,1,NULL,NULL),(2051,'Выводные','2048',NULL,1,NULL,NULL),(3,'Керамические smd','2001','3',0,'2001',NULL),(4,'Керамические выводные','2001','3',0,'2001',NULL),(5,'Электролитические smd','2001','3',0,'2001',NULL),(6,'Электролитические выводные','2001','3',0,'2001',NULL),(7,'Тумблеры','2042','16',0,'2001',NULL),(8,'Трансформаторы тока','2041','37',0,'2001',NULL),(9,'Трансформаторы напряжения','2041','37',0,'2001',NULL),(10,'Трансформаторы прочие','2041','37',0,'2001',NULL),(11,'Диоды Шоттки','2050','5',0,'2001',NULL),(12,'Диоды выпрямительные','2050','5',0,'2001',NULL),(13,'Диоды прочие','2050','5',0,'2001',NULL),(14,'Транзисторы биполярные','2080','12',0,'2001',NULL),(15,'Транзисторы полевые','2080','12',0,'2001',NULL),(16,'Резисторные сборки','2110','11',0,'2001',NULL),(17,'Резисторы smd','2110','11',0,'2001',NULL),(18,'Резисторы выводные','2110','11',0,'2001',NULL),(19,'Резисторы переменные','2110','11',0,'2001',NULL),(20,'Резисторы подстроечные','2110','11',0,'2001',NULL),(21,'Разъёмы на кабель','2300','4',0,'2001',NULL),(22,'Разъёмы на плату','2300','4',0,'2001',NULL),(23,'Разъёмы прочие','2300','4',0,'2001',NULL),(24,'М/сх АЦП и ЦАП','2400','36',0,'2001',NULL),(25,'М/сх аналоговые','2400','35',0,'2001',NULL),(26,'М/сх аналоговые ключи','2400','22',0,'2001',NULL),(27,'М/сх интерфейсные','2400','24',0,'2001',NULL),(28,'Светодиоды','2350','9',0,'2001',NULL),(29,'Оптоэлектроника','2350','9',0,'2001',NULL),(30,'М/сх изоляторы','2400','8',0,'2001',NULL),(31,'М/сх логика','2400','',0,'2001',NULL),(32,'М/сх память','2400','27',0,'2001',NULL),(33,'М/сх микроконтроллеры','2400','26',0,'2001',NULL),(34,'М/сх операционные усилители','2400','15',0,'2001',NULL),(35,'М/сх управления питанием','2400','6',0,'2001',NULL),(36,'М/сх оптоэлектроника','2400','23',0,'2001',NULL),(37,'М/сх защитные','2400','13',0,'2001',NULL),(38,'М/сх источников напряжения','2400','7',0,'2001',NULL),(1,'Индуктивности','0','25',0,'2001',NULL),(2,'Модули','0','17',0,'2001',NULL),(40,'Предохранители самовосстанавливающиеся','0','30',0,'2001',NULL);
/*!40000 ALTER TABLE `classes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `components`
--

DROP TABLE IF EXISTS `components`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `components` (
  `idcomponents` int(11) NOT NULL,
  `alias` varchar(45) NOT NULL,
  `idalias` varchar(45) NOT NULL,
  `db` varchar(45) NOT NULL,
  `dbs` varchar(45) NOT NULL,
  `par1name` varchar(45) NOT NULL,
  `par2name` varchar(45) DEFAULT NULL,
  `par3name` varchar(45) DEFAULT NULL,
  `par4name` varchar(45) DEFAULT NULL,
  `par5name` varchar(45) DEFAULT NULL,
  `unitset` int(11) NOT NULL,
  PRIMARY KEY (`idcomponents`),
  UNIQUE KEY `idcomponents_UNIQUE` (`idcomponents`)
) ENGINE=MyISAM AUTO_INCREMENT=29 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `components`
--

LOCK TABLES `components` WRITE;
/*!40000 ALTER TABLE `components` DISABLE KEYS */;
INSERT INTO `components` VALUES (3,'Конденсаторы','0','altium','capasitors','Ёмкость','Ном. напряжение','NULL','NULL','NULL',2),(4,'Разъёмы','0','altium','connectors','Количество контактов','Рабочий ток','Напряжение изоляции','NULL','NULL',0),(5,'Диоды','0','altium','diodes','Падение напряжения','Рабочий ток','Скорость переключения','Обр. напряжение','NULL',0),(6,'Управление питанием','18','altium','IC_PowerMgmt','Входное напряжение','Выходное напряжение','Рабочий ток','Рабочая частота','Падение напряжения',0),(8,'Механические компоненты','0','altium','mechanical','Напряжение изоляции','NULL','NULL','NULL','NULL',0),(9,'Оптоэлектроника','0','altium','opto','Цвет свечения','Падение напряжения','Количество в корпусе','NULL','NULL',0),(10,'Источники питания','0','altium','power','Входное напряжение','Выходное напряжение','Количество выходов','NULL','NULL',0),(11,'Резисторы','0','altium','resistors','Сопротивление','Напряжение изоляции','NULL','NULL','NULL',1),(12,'Транзисторы','0','altium','transistors','Рабочий ток','Обр. напряжение','Коэфф. усиления','Скорость переключения','NULL',0),(14,'Элементы без номенклатуры','0','altium','connections','','NULL','NULL','NULL','NULL',0),(7,'Источники опорного напряжения','18','altium','IC_VReference','Входное напряжение','Выходное напряжение','Выходной ток','NULL','NULL',0),(16,'Устройства коммутации','0','altium','switches','Количество в корпусе','Рабочий ток','Напряжение изоляции','NULL','NULL',0),(17,'Модули','0','altium','modules','Тип','','NULL','NULL','NULL',0),(18,'Микросхемы','0','','','','NULL','NULL','NULL','NULL',0),(19,'Защитные компоненты','0','','','','NULL','NULL','NULL','NULL',0),(20,'Варисторы','19','altium','varistors','Ном. напряжение RMS','Ном. напряжение DC','Энергия','Ток (8/20)','NULL',0),(21,'Защитные диоды','19','altium','tvs','Ном. напряжение','Напряжение пробоя','Ток (10/1000)','NULL','NULL',0),(22,'Ключи аналоговые','18','altium','IC_AnSwitch','Кол-во каналов','Проходное сопротивление','Рабочее напряжение','Скорость переключения','Тип',0),(23,'Оптроны','18','altium','IC_Opto','Падение напряжения','Макс. обр. напряжение','Скорость переключения','Коэффициент передачи по току','Тип',0),(24,'Интерфейсы','18','altium','IC_Interface','Скорость передачи','Тип','Количество каналов','NULL','NULL',0),(15,'Операционные усилители','18','altium','IC_OpAmp','Рабочее напряжение','Скорость нарастания','Коэфф. усиления','Напряжение смещения','КОСС',0),(13,'Защита','18','altium','IC_Protection','Кол-во каналов','Ном. напряжение','Применение','NULL','NULL',0),(25,'Индуктивности','0','altium','inductors','Индуктивность','Рабочий ток','Добротность','Сопротивление','NULL',0),(26,'Микроконтроллеры','18','altium','IC_Microcontroller','Тактовая частота','Архитектура','ОЗУ','ПЗУ','Напряжение питания',0),(27,'Память','18','altium','IC_Memory','Ёмкость','Ном. напряжение','Скорость записи','Скорость чтения','Тип',0),(28,'Генераторы','0','altium','Oscillators','Частота','Тип','Ёмкость нагрузки','Напряжение питания','NULL',0),(30,'Позисторы','19','altium','Posistors','Рабочий ток','Сопр. в открытом состоянии','Время срабатывания','Рабочее напряжение','NULL',0),(29,'Разрядники','19','altium','GDTs','Ном. напряжение','Напряжение горения дуги','NULL','NULL','NULL',0),(1,'Наименование','pc.sup.components','','','Параметр 1','Параметр 2','Параметр 3','Параметр 4','Параметр 5',0),(2,'Наименование','Ссылка','БД','Таблица','Наименование 1-го параметра','Наименование 2-го параметра','Наименование 3-го параметра','Наименование 4-го параметра','Наименование 5-го параметра',0),(35,'Аналоговые (разное)','18','altium','IC_AnMixed','Описание','Ном. напряжение','Параметр 3','Параметр 4','Параметр 5',0),(36,'АЦП и ЦАП','18','altium','IC_ADCDAC','Разрешение (бит)','Скорость оцифровки','Опорное напряжение','Количество каналов','Тип цифрового выхода',0),(37,'Трансформаторы','0','altium','transform','Коэффициент трансформации','Максимальный первичный ток/напряжение','Сопротивление обмотки','Точность по амплитуде','Точность по углу',0),(38,'Изоляторы','18','altium','IC_Isolator','Напряжение изоляции','Напряжение питания','Кол-во каналов на стороне 1','Кол-во каналов на стороне 2','Максимальная скорость',0);
/*!40000 ALTER TABLE `components` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `constr`
--

DROP TABLE IF EXISTS `constr`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `constr` (
  `idconstr` int(11) NOT NULL AUTO_INCREMENT,
  `idmanuf` varchar(45) DEFAULT NULL,
  `pn` varchar(45) NOT NULL,
  `decnumber` varchar(45) DEFAULT NULL,
  `gost` varchar(45) DEFAULT NULL,
  `rev` varchar(45) DEFAULT NULL,
  `description` varchar(128) DEFAULT NULL,
  `notes` varchar(45) DEFAULT NULL,
  `helpurl` varchar(255) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` varchar(45) DEFAULT '0',
  `date` datetime DEFAULT NULL,
  PRIMARY KEY (`idconstr`),
  UNIQUE KEY `idconstr_UNIQUE` (`idconstr`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `constr`
--

LOCK TABLES `constr` WRITE;
/*!40000 ALTER TABLE `constr` DISABLE KEYS */;
/*!40000 ALTER TABLE `constr` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `contacts`
--

DROP TABLE IF EXISTS `contacts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `contacts` (
  `idcontacts` int(11) NOT NULL,
  `contacts` varchar(128) DEFAULT NULL,
  `company` varchar(45) DEFAULT NULL,
  `position` varchar(45) DEFAULT NULL,
  `e-mail1` varchar(45) DEFAULT NULL,
  `e-mail2` varchar(45) DEFAULT NULL,
  `phonewrk` varchar(45) DEFAULT NULL,
  `phonemob` varchar(45) DEFAULT NULL,
  `phonemob2` varchar(45) DEFAULT NULL,
  `phonefax` varchar(45) DEFAULT NULL,
  `comment` varchar(256) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  PRIMARY KEY (`idcontacts`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `contacts`
--

LOCK TABLES `contacts` WRITE;
/*!40000 ALTER TABLE `contacts` DISABLE KEYS */;
/*!40000 ALTER TABLE `contacts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `contractors`
--

DROP TABLE IF EXISTS `contractors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `contractors` (
  `idcontractors` int(11) NOT NULL,
  `contractors` varchar(45) DEFAULT NULL,
  `country` varchar(45) DEFAULT NULL,
  `index` varchar(45) DEFAULT NULL,
  `city` varchar(45) DEFAULT NULL,
  `address` varchar(128) DEFAULT NULL,
  `phone` varchar(45) DEFAULT NULL,
  `manager` varchar(45) DEFAULT NULL,
  `phonemgr` varchar(45) DEFAULT NULL,
  `inn` varchar(45) DEFAULT NULL,
  `kpp` varchar(45) DEFAULT NULL,
  `benefacc` varchar(45) DEFAULT NULL,
  `benefbank` varchar(45) DEFAULT NULL,
  `bik` varchar(45) DEFAULT NULL,
  `corracc` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  `idpers` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`idcontractors`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `contractors`
--

LOCK TABLES `contractors` WRITE;
/*!40000 ALTER TABLE `contractors` DISABLE KEYS */;
INSERT INTO `contractors` VALUES (1,'Наименование','Страна','Индекс','Город','Адрес','Телефон','Менеджер','Телефон менеджера','ИНН','КПП','Расч. счёт','Банк','БИК','Корр. счёт',0,NULL),(2,'Наименование','Страна','Индекс','Город','Адрес','Телефон общий','ФИО менеджера','Телефон менеджера','ИНН','КПП','Расч. счёт','Банк с основным счётом','БИК','Корр. счёт',0,NULL),(3,'ООО АСУ-ВЭИ','Россия','111024','Москва','2-я Кабельная ул., д. 2','+74957858826','Афанасьев А.Д.','+74957858826','7722017180','772201001','40702810738120102490','Московский банк Сбербанка России ОАО  г. Моск','044525225','30101810400000000225',0,NULL);
/*!40000 ALTER TABLE `contractors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `currency`
--

DROP TABLE IF EXISTS `currency`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `currency` (
  `idcurrency` int(11) NOT NULL,
  `currency` varchar(128) DEFAULT NULL,
  `code` varchar(128) DEFAULT NULL,
  `abbreviation` varchar(128) DEFAULT NULL,
  `date` datetime DEFAULT NULL,
  `deleted` int(1) NOT NULL DEFAULT '0',
  `idpers` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`idcurrency`),
  UNIQUE KEY `idcurrency_UNIQUE` (`idcurrency`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `currency`
--

LOCK TABLES `currency` WRITE;
/*!40000 ALTER TABLE `currency` DISABLE KEYS */;
INSERT INTO `currency` VALUES (1,'Наименование','Код валюты','Сокращение валюты','0000-00-00 00:00:00',0,NULL),(2,'Наименование','Код валюты','Сокращение валюты','0000-00-00 00:00:00',0,NULL),(3,'Рубль','643','RUB',NULL,0,NULL),(4,'Доллар США','840','USD',NULL,0,NULL),(5,'Евро','978','EUR',NULL,0,NULL);
/*!40000 ALTER TABLE `currency` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `devices`
--

DROP TABLE IF EXISTS `devices`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `devices` (
  `iddev` int(11) NOT NULL AUTO_INCREMENT,
  `idsp` varchar(45) DEFAULT NULL,
  `idmanuf` varchar(45) DEFAULT NULL,
  `pn` varchar(45) NOT NULL,
  `isobsolete` varchar(45) DEFAULT NULL,
  `description` varchar(64) DEFAULT NULL,
  `notes` varchar(45) DEFAULT NULL,
  `url` varchar(255) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) DEFAULT '0',
  `date` datetime DEFAULT NULL,
  PRIMARY KEY (`iddev`),
  UNIQUE KEY `iddev_UNIQUE` (`iddev`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `devices`
--

LOCK TABLES `devices` WRITE;
/*!40000 ALTER TABLE `devices` DISABLE KEYS */;
/*!40000 ALTER TABLE `devices` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `documents`
--

DROP TABLE IF EXISTS `documents`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `documents` (
  `iddocuments` int(11) NOT NULL,
  `documents` varchar(128) DEFAULT NULL,
  `scanpath` varchar(128) DEFAULT NULL,
  `idsupplier` varchar(10) DEFAULT NULL,
  `idcustomer` varchar(10) DEFAULT NULL,
  `reason` varchar(2) DEFAULT NULL,
  `idpers` varchar(10) DEFAULT NULL,
  `direction` varchar(1) DEFAULT NULL,
  `value` varchar(128) DEFAULT NULL,
  `date` varchar(128) DEFAULT NULL,
  `deleted` int(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`iddocuments`),
  UNIQUE KEY `iddocuments_UNIQUE` (`iddocuments`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `documents`
--

LOCK TABLES `documents` WRITE;
/*!40000 ALTER TABLE `documents` DISABLE KEYS */;
INSERT INTO `documents` VALUES (1,'Номер ТН','','pc.ent.con','pc.ent.con','pc','pc.sup.per','','Сумма','0000-00-00 00:00:00',0),(2,'Номер ТН','Путь к файлу ТН','Поставщик','Получатель','Ос','ФИО создав','Н','Сумма','0000-00-00 00:00:00',0),(4,'4','','3','3','4','2001','o','0.00','06/10/2014',0),(3,'3','','3','3','4','2001','i','0.00','06/10/2014',0),(5,'5','','3','3','4','2001','i','1239.24','21/10/2014',0);
/*!40000 ALTER TABLE `documents` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `flow`
--

DROP TABLE IF EXISTS `flow`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `flow` (
  `idflow` int(11) NOT NULL,
  `flow` varchar(128) DEFAULT NULL,
  `idnk` varchar(128) DEFAULT NULL,
  `idqnk` varchar(128) DEFAULT NULL,
  `quantity` varchar(128) DEFAULT NULL,
  `idunitset` varchar(128) DEFAULT NULL,
  `price` varchar(128) DEFAULT NULL,
  `idcurrency` varchar(128) DEFAULT NULL,
  `idnkwh` varchar(128) DEFAULT NULL,
  `date` datetime DEFAULT NULL,
  `deleted` int(1) NOT NULL DEFAULT '0',
  `idpers` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idflow`),
  UNIQUE KEY `idflow_UNIQUE` (`idflow`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `flow`
--

LOCK TABLES `flow` WRITE;
/*!40000 ALTER TABLE `flow` DISABLE KEYS */;
INSERT INTO `flow` VALUES (1,'pc.ent.documents','pc.ent.nk',NULL,'','','','','pc.ent.nkwh','0000-00-00 00:00:00',0,NULL),(2,'Номер документа','Наименование',NULL,'Количество','Единицы измерения','Цена (б/ НДС)','Валюта','Местонахождение','0000-00-00 00:00:00',0,NULL),(4,'3','0.2087',NULL,'1','12','','3','4',NULL,0,NULL),(3,'3','0.2088',NULL,'2','12','','3','3',NULL,0,NULL),(5,'3','0.2046',NULL,'1','12','','3','5',NULL,0,NULL);
/*!40000 ALTER TABLE `flow` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ica`
--

DROP TABLE IF EXISTS `ica`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ica` (
  `idica` int(11) NOT NULL AUTO_INCREMENT,
  `idbase` varchar(45) DEFAULT NULL,
  `idchg` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT NULL,
  `idpers` varchar(128) DEFAULT NULL,
  `date` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`idica`),
  UNIQUE KEY `idica_UNIQUE` (`idica`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ica`
--

LOCK TABLES `ica` WRITE;
/*!40000 ALTER TABLE `ica` DISABLE KEYS */;
/*!40000 ALTER TABLE `ica` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `manuf`
--

DROP TABLE IF EXISTS `manuf`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `manuf` (
  `idmanuf` int(11) NOT NULL AUTO_INCREMENT,
  `manuf` varchar(45) NOT NULL,
  `www` varchar(255) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` varchar(45) DEFAULT '0',
  `date` datetime DEFAULT NULL,
  PRIMARY KEY (`idmanuf`),
  UNIQUE KEY `idmanuf_UNIQUE` (`idmanuf`)
) ENGINE=MyISAM AUTO_INCREMENT=2039 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `manuf`
--

LOCK TABLES `manuf` WRITE;
/*!40000 ALTER TABLE `manuf` DISABLE KEYS */;
INSERT INTO `manuf` VALUES (2001,'Analog Devices','www.analog.com','0','0',NULL),(2002,'Fairchild Semiconductor','www.fairchildsemi.com','1','0','2013-10-14 21:28:40'),(2003,'ООО АСУ-ВЭИ','www.asu-vei.ru','0','0',NULL),(2007,'Littelfuse','www.littelfuse.com','1','0','2013-09-25 17:51:36'),(2008,'ST Microelectronics','www.st.com','1','0','2013-09-25 17:53:38'),(2009,'Tyco Electronics','www.te.com','1','0','2013-09-25 18:01:59'),(2010,'Murata','www.murata.com','1','0','2013-09-30 15:14:01'),(2011,'НКП','www.china.com','1','0','2013-10-31 09:17:01'),(2012,'Epcos AG','www.epcos.com','2001','0','2013-10-31 17:19:20'),(2013,'ON Semiconductor','www.onsemi.com','2001','0','2013-10-31 17:32:07'),(2014,'Meanwell','www.meanwell.com','2001','0','2014-02-15 18:01:11'),(2015,'Linear Technology','www.linear.com','2001','0','2013-10-31 17:44:21'),(2016,'Toshiba Electronics','www.toshiba-components.com','2001','0','2013-10-31 18:03:15'),(2017,'Kingbright Electronic Co, Ltd','www.kingbright.com','2001','0','2013-10-31 18:05:00'),(2018,'WAGO Kontakttechnik GmbH & Co. KG','www.wago.com','2001','0','2013-10-31 18:10:40'),(2019,'Yageo','www.yageo.com','2001','0','2013-11-07 14:18:22'),(2020,'Panasonic','www.panasonic.com/industrial','2001','0','2014-01-15 17:48:38'),(2021,'Micrel','www.micrel.com','2001','0','2014-01-27 14:46:08'),(2022,'Texas Instruments','www.ti.com','2008','0','2014-01-27 18:01:35'),(2023,'Rohm','www.rohm.com','2001','0','2014-02-03 10:56:32'),(2024,'Maxim Integrated','www.maximintegrated.com','2008','0','2014-02-04 15:06:00'),(2025,'Bourns Inc','www.bourns.com','2001','0','2014-02-05 15:57:09'),(2026,'КБ Икар','http://www.kbikar.ru','2008','0','2014-02-06 11:58:49'),(2027,'Atmel','http://www.atmel.com','2008','0','2014-02-10 10:26:50'),(2028,'Geyer Electronic','http://www.geyer-electronic.de','2008','0','2014-02-19 11:43:48'),(2029,'Traco','http://www.tracopower.com','2008','0','2014-03-03 16:37:24'),(2030,'Тес1','цуфвыф','2011','1','2014-04-10 18:50:49'),(2031,'Dinkle','http://www.dinkle.org','2008','0','2014-05-20 17:02:13'),(2032,'Phoenix Contact','https://www.phoenixcontact.com','2008','0','2014-05-20 17:04:23'),(2033,'ICOP','icop.com.tw','2001','0','2014-05-28 14:54:12'),(2034,'NXP','http://www.nxp.com','2008','0','2014-06-27 10:11:13'),(2035,'Weidmuller','http://www.weidmueller.ru/','2008','0','2014-06-30 16:17:16'),(2036,'Amphenol','','2010','0','2014-07-09 13:58:09'),(2037,'ОАО Эркон','www.erkon-nn.ru','2010','0','2014-07-11 13:37:05'),(2038,'Molex','www.molex.com','2008','0','2014-07-23 18:08:43');
/*!40000 ALTER TABLE `manuf` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `nk`
--

DROP TABLE IF EXISTS `nk`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `nk` (
  `idnk` int(11) NOT NULL AUTO_INCREMENT,
  `nk` varchar(64) NOT NULL,
  `idclasses` varchar(45) DEFAULT NULL,
  `idmanuf` varchar(45) DEFAULT NULL,
  `idalt` varchar(45) DEFAULT NULL,
  `idsdw` varchar(45) DEFAULT NULL,
  `idsge` varchar(45) DEFAULT NULL,
  `idctr` varchar(45) DEFAULT NULL,
  `iddev` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` varchar(45) DEFAULT '0',
  `date` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idnk`),
  UNIQUE KEY `idnk_UNIQUE` (`idnk`)
) ENGINE=MyISAM AUTO_INCREMENT=2184 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `nk`
--

LOCK TABLES `nk` WRITE;
/*!40000 ALTER TABLE `nk` DISABLE KEYS */;
INSERT INTO `nk` VALUES (2011,'1-1571983-0','2700','2009','1','0','0',NULL,NULL,'2001','0','2013-10-29 20:18:26'),(2013,'Чип Рез. 0,063 Вт-1 кОм-5%,0603','2113','2011','7','0','0',NULL,NULL,'2001','0','2014-02-15 18:58:51'),(2014,'12312','2050','2001','7','0','0',NULL,NULL,'2001','1','2013-11-05 16:49:52'),(2015,'Чип Рез.0,063-100 Ом-5%,0603 (1608)','2110','2011','8','0','0',NULL,NULL,'2001','0','2013-11-05 16:53:46'),(2016,'YC158TJR-0710K(L)','2200','2019','7','0','0',NULL,NULL,'2001','0','2013-11-07 14:31:54'),(2017,'RC0603FR-0712K4(L)','2113','2019','10','0','0',NULL,NULL,'2001','0','2013-11-11 15:10:06'),(2018,'Чип Рез. 0,063 Вт-12,4 кОм-1%,0603','2113','2011','11','0','0',NULL,NULL,'2001','0','2013-11-11 15:12:30'),(2019,'5177983-1','2300','2009','5','0','0',NULL,NULL,'2001','0','2013-11-18 09:46:46'),(2020,'DC-20/20YWA','2350','2017','2','0','0',NULL,NULL,'2001','0','2013-12-26 10:15:29'),(2021,'Чип Рез. 0,063 Вт-4,7 кОм-5%,0603','2113','2011','12','0','0',NULL,NULL,'2001','0','2014-01-16 15:58:51'),(2022,'YC164-JR-07220R','2200','2019','13','0','0',NULL,NULL,'2001','0','2014-01-15 17:21:10'),(2023,'EXB-38V221JV','2200','2020','13','0','0',NULL,NULL,'2001','0','2014-01-15 17:49:48'),(2024,'Чип Рез. 0,063 Вт-3,3 кОм-5%,0603','2113','2011','14','0','0',NULL,NULL,'2001','0','2014-01-17 15:28:20'),(2025,'B72220S0110K101','2700','2012','6','0','0',NULL,NULL,'2008','0','2014-01-20 13:49:40'),(2026,'BAV99','2050','2002','3','0','0',NULL,NULL,'2008','0','2014-01-24 15:07:15'),(2027,'ADG706BRU','2400','2001','1','0','0',NULL,NULL,'2001','0','2014-01-30 11:27:02'),(2028,'OPA4340UA','2400','2022','1','0','0',NULL,NULL,'2001','0','2014-01-30 11:27:10'),(2029,'BD9778HFP','2400','2023','2','0','0',NULL,NULL,'2001','0','2014-02-03 11:04:39'),(2030,'LTC4415IDHC','2400','2015','6','0','0',NULL,NULL,'2008','0','2014-02-04 12:54:57'),(2031,'USBLC6','2400','2008','1','0','0',NULL,NULL,'2008','0','2014-02-04 13:30:20'),(2032,'MAX6070AAUT25+','2400','2024','1','0','0',NULL,NULL,'2008','0','2014-02-04 15:16:58'),(2033,'Чип Рез. 0,063 Вт-100кОм-5%,0603','2113','2011','15','0','0',NULL,NULL,'2008','0','2014-02-05 11:13:30'),(2034,'Чип Рез. 0,063 Вт-22 кОм-5%,0603','2113','2011','16','0','0',NULL,NULL,'2008','0','2014-02-05 11:45:23'),(2035,'Чип Рез. 0,063 Вт-750 Ом-5%,0603','2113','2011','17','0','0',NULL,NULL,'2008','0','2014-02-05 11:52:31'),(2036,'SRR1208-101Y','2700','2025','1','0','0',NULL,NULL,'2001','0','2014-02-05 16:00:48'),(2037,'STPS3L60S','2050','2008','8','0','0',NULL,NULL,'2001','0','2014-02-06 11:12:19'),(2038,'Р1-16М-0,125(1206) А -50кОм+0,5%-0,5 Г','2113','2026','18','0','0',NULL,NULL,'2008','0','2014-02-06 13:52:56'),(2039,'Р1-16М-0,125(1206) А -10кОм+0,5%-0,5 Г','2113','2026','19','0','0',NULL,NULL,'2008','0','2014-02-06 13:56:00'),(2040,'Р1-16М-0,125(1206) А -250 Ом+0,5%-0,5 Г','2113','2026','20','0','0',NULL,NULL,'2008','0','2014-02-06 14:01:05'),(2041,'Чип Конд. 220 мкФ,10%,тип D','2008','2011','6','0','0',NULL,NULL,'2001','0','2014-02-06 14:18:18'),(2042,'AT25DF641-S3H','2400','2027','1','0','0',NULL,NULL,'2008','0','2014-02-10 10:34:46'),(2043,'EEEFC1V221','2008','2020','8','0','0',NULL,NULL,'2001','0','2014-02-11 12:27:38'),(2044,'5747847-4','2300','2009','6','0','0',NULL,NULL,'2008','0','2014-02-11 13:20:57'),(2045,'Чип Конд. 1 мкФ,50 В,10%,тип 1206','2009','2011','9','0','0',NULL,NULL,'2001','0','2014-02-12 11:23:36'),(2046,'Чип Рез. 0,1 Вт-390 кОм-5%,тип 0603','2113','2011','21','0','0',NULL,NULL,'2001','0','2014-02-12 11:41:43'),(2047,'B72220S0500K101','2047','2012','','','','','','2001','0','31/05/2015 19:24:03'),(2048,'Чип Конд. 1000 пФ,25 В,10%,тип 0603','2009','2011','10','0','0',NULL,NULL,'2001','0','2014-02-12 17:46:53'),(2049,'Чип Рез. 0,1 Вт-150 кОм-5%,тип 0603','2113','2011','22','0','0',NULL,NULL,'2001','0','2014-02-12 18:08:01'),(2050,'Чип Рез. 0,1 Вт-12 кОм-1%,тип 0603','2113','2011','23','0','0',NULL,NULL,'2001','0','2014-02-13 08:14:57'),(2051,'Чип Рез. 0,1 Вт-27 кОм-1%,тип 0603','2113','2011','24','0','0',NULL,NULL,'2001','0','2014-02-13 08:16:24'),(2052,'Чип Рез. 0,125 Вт-22 Ом-5%,0805','2113','2011','25','0','0',NULL,NULL,'2008','0','2014-02-13 09:50:18'),(2053,'Чип Рез. 0,125 Вт-4,87 кОм-1%,0805','2113','2011','26','0','0',NULL,NULL,'2008','0','2014-02-13 10:04:36'),(2054,'Чип Конд. 2200 пФ,X7R,0603','2009','2011','11','0','0',NULL,NULL,'2008','0','2014-02-13 10:20:30'),(2055,'KSZ8081RNAI','2400','2021','6','0','0',NULL,NULL,'2008','0','2014-02-13 14:10:40'),(2056,'FDN338P','2080','2002','2','0','0',NULL,NULL,'2001','0','2014-02-13 15:19:59'),(2057,'Р1-16М-0,062(0805) А -20кОм+0,5%-0,5 Д','2113','2026','27','0','0',NULL,NULL,'2008','0','2014-02-17 15:00:45'),(2058,'SMBJ12A','2650','2007','2','0','0',NULL,NULL,'2008','0','2014-02-17 16:49:50'),(2059,'STM32F407VET6','2400','2008','1','0','0',NULL,NULL,'2008','0','2014-02-18 09:35:19'),(2060,'Чип тант. Конд. 22 мкФ,10 В,10%,тип B','2008','2011','12','0','0',NULL,NULL,'2001','0','2014-02-18 10:06:56'),(2061,'Чип Конд. 10 пФ,NPO,0603','2009','2011','13','0','0',NULL,NULL,'2008','0','2014-02-19 15:43:27'),(2062,'Чип Рез. 0,063 Вт-0 Ом-5%,0603','2113','2011','28','0','0',NULL,NULL,'2008','0','2014-02-19 15:50:48'),(2063,'Чип Конд. 2,2 мкФ,X7R,1206','2009','2011','15','0','0',NULL,NULL,'2008','0','2014-02-20 11:02:06'),(2064,'Чип Рез. 0,125 Вт-6,49 кОм-1%,0805','2113','2011','29','0','0',NULL,NULL,'2008','0','2014-02-20 11:35:11'),(2065,'MH1608-221Y','2700','2025','6','0','0',NULL,NULL,'2001','0','2014-02-20 11:48:42'),(2067,'Чип Конд. 33 пФ,50 В,NP0,тип 0603','2009','2011','16','0','0',NULL,NULL,'2001','0','2014-02-20 17:08:21'),(2068,'Чип Конд. 18 пФ,NP0,тип 0603','2009','2011','17','0','0',NULL,NULL,'2001','0','2014-02-20 17:08:48'),(2069,'Чип Рез. 0,1 Вт-49,9 Ом-1%,тип 0603','2113','2011','30','0','0',NULL,NULL,'2001','0','2014-02-21 15:44:36'),(2070,'NCP1529','2400','2013','7','0','0',NULL,NULL,'2008','0','2014-02-24 15:54:37'),(2071,'Чип Рез. 0,1 Вт-1,2 МОм-тип 0603','2113','2011','32','0','0',NULL,NULL,'2008','0','2014-02-24 17:04:35'),(2072,'Чип Рез. 0,1 Вт-270 кОм-1%,тип 0603','2113','2011','33','0','0',NULL,NULL,'2008','0','2014-02-24 17:18:01'),(2073,'Чип Конд. 4,7 мкФ,тип 0805','2009','2011','18','0','0',NULL,NULL,'2008','0','2014-02-24 17:38:45'),(2074,'ITA6V5B3','2400','2008','6','0','0',NULL,NULL,'2008','0','2014-02-25 14:12:41'),(2075,'Чип Рез. 0,25 Вт-51 Ом-тип 1206','2113','2011','34','0','0',NULL,NULL,'2001','0','2014-02-26 14:27:08'),(2076,'Чип Конд. 6,8 пФ,тип 0805','2009','2011','19','0','0',NULL,NULL,'2008','0','2014-02-26 14:42:29'),(2077,'Чип Рез. 0,1 Вт-1 кОм-5%,тип 0603','2113','2011','35','0','0',NULL,NULL,'2001','0','2014-02-26 16:15:54'),(2078,'L-914CK/4GDT','2350','2017','6','0','0',NULL,NULL,'2008','0','2014-02-27 16:40:49'),(2079,'Чип Рез. 0,125 Вт-180 Ом-тип 0603','2113','2011','36','0','0',NULL,NULL,'2008','0','2014-02-27 17:14:13'),(2080,'1825910-7','2700','2009','9','0','0',NULL,NULL,'2008','0','2014-02-27 19:11:43'),(2081,'SP0305BAHTG','2400','2007','7','0','0',NULL,NULL,'2008','0','2014-02-27 20:10:01'),(2082,'Вилка на плату B4B-PH-K-s','2300','2011','9','0','0',NULL,NULL,'2008','0','2014-02-28 11:18:10'),(2083,'Чип Рез. 1 Вт-270 Ом-5%,тип 2512','2113','2011','37','0','0',NULL,NULL,'2008','0','2014-03-03 14:54:13'),(2084,'Штыревой разъем на плату PLS-1','2300','2011','10','0','0',NULL,NULL,'2008','0','2014-03-04 11:36:16'),(2085,'SKE15B-24','2600','2014','2','0','0',NULL,NULL,'2008','0','2014-03-04 13:59:23'),(2086,'OPA340NA/250','2400','2022','6','0','0',NULL,NULL,'2008','0','2014-03-05 14:31:04'),(2087,'5-5179009-1','2300','2009','11','0','0',NULL,NULL,'2001','0','2014-03-06 17:02:29'),(2088,'1825006-7','2043','2009','10','0','0',NULL,NULL,'2008','0','2014-03-11 15:48:22'),(2089,'1571983-1','2043','2009','11','0','0',NULL,NULL,'2008','0','2014-03-11 18:16:50'),(2090,'7914G-1-000E','2044','2025','12','0','0',NULL,NULL,'2008','0','2014-04-01 13:17:32'),(2091,'AT25DB081D-SU','2400','2027','6','0','0',NULL,NULL,'2008','0','2014-04-01 16:30:06'),(2092,'EECS5R5V105','2004','2020','20','0','0',NULL,NULL,'2008','0','2014-04-02 11:33:20'),(2093,'Батарейный отсек вертикальный с батарейкой CR2032','2300','2011','12','0','0',NULL,NULL,'2008','0','2014-04-02 18:00:44'),(2094,'BAT54C','2050','2002','9','0','0',NULL,NULL,'2008','0','2014-04-02 18:36:14'),(2095,'1734753-1','2300','2009','13','0','0',NULL,NULL,'2008','0','2014-04-03 11:29:01'),(2096,'106081-1','2300','2009','14','0','0',NULL,NULL,'2001','0','2014-04-11 10:15:44'),(2097,'5223955-2','2300','2009','15','0','0',NULL,NULL,'2001','0','2014-04-11 10:30:38'),(2098,'120953-5','2300','2009','16','0','0',NULL,NULL,'2001','0','2014-04-11 10:36:44'),(2099,'223956-1','2500','2009','2','0','0',NULL,NULL,'2001','0','2014-04-11 11:03:14'),(2100,'5177984-1','2300','2009','17','0','0',NULL,NULL,'2001','0','2014-04-14 13:29:07'),(2101,'L-7104MD/GD','2350','2017','7','0','0',NULL,NULL,'2008','0','2014-04-15 18:12:00'),(2102,'Штыревой разъем на плату PLS-2 с джампером MJ-O-6','2300','2011','18','0','0',NULL,NULL,'2008','0','2014-04-18 14:42:46'),(2103,'Чип Рез. 0,125 Вт-10 кОм-0,5%,тип 0805','2113','2011','38','0','0',NULL,NULL,'2008','0','2014-05-14 17:34:02'),(2104,'Чип Рез. 0,5 Вт-100 Ом-тип 1210','2113','2011','39','0','0',NULL,NULL,'2008','0','2014-05-19 14:12:51'),(2105,'SMBJ5.0A','2650','2007','9','0','0',NULL,NULL,'2008','0','2014-05-19 15:32:30'),(2106,'SMBJ9.0','2650','2007','10','0','0',NULL,NULL,'2008','0','2014-05-19 16:08:59'),(2107,'SMBJ13CA','2650','2007','11','0','0',NULL,NULL,'2008','0','2014-05-19 16:46:15'),(2108,'MF-MSMF014','2700','2025','1','0','0',NULL,NULL,'2008','0','2014-05-20 12:51:39'),(2109,'Гнездо на плату PBD-64 2,54мм','2300','2011','19','0','0',NULL,NULL,'2008','0','2014-05-20 13:54:16'),(2110,'Гнездо на плату PBD-40 2,54мм','2300','2011','20','0','0',NULL,NULL,'2008','0','2014-05-20 14:08:56'),(2111,'2035-09-SM-RPLF','2049','2025','1','0','0',NULL,NULL,'2008','0','2014-05-20 14:51:22'),(2112,'MKDSO 2,5/2-L','2300','2032','21','0','0',NULL,NULL,'2008','0','2014-05-20 17:14:47'),(2113,'MKDSO2,5/ 2-R','2300','2032','22','0','0',NULL,NULL,'2008','0','2014-05-20 17:16:35'),(2114,'1825006-3','2043','2009','13','0','0',NULL,NULL,'2008','0','2014-05-22 14:41:54'),(2115,'Чип конд. электролит. 1000 мкФх10 В (1010)','2008','2019','21','0','0',NULL,NULL,'2001','0','2014-05-23 16:20:17'),(2116,'Чип Рез. 0,5 Вт-18 кОм-тип 2010','2113','2011','40','0','0',NULL,NULL,'2008','0','2014-05-23 16:35:28'),(2117,'Чип Рез. 0,25 Вт-180 кОм-5%,тип 1206','2113','2011','41','0','0',NULL,NULL,'2008','0','2014-05-23 16:46:55'),(2118,'Чип вилка на плату BHS2-20','2300','2011','23','0','0',NULL,NULL,'2001','0','2014-05-23 17:13:51'),(2119,'1N5378B','2050','2013','10','0','0',NULL,NULL,'2008','0','2014-05-23 18:04:58'),(2120,'Чип Рез. 0,125 Вт-6,8 кОм-тип 0805','2113','2011','42','0','0',NULL,NULL,'2008','0','2014-05-23 18:24:29'),(2121,'KP-1608ID','2350','2017','8','0','0',NULL,NULL,'2008','0','2014-05-26 12:37:53'),(2122,'TLP182','2350','2016','9','0','0',NULL,NULL,'2008','0','2014-05-26 12:55:03'),(2123,' Конд. 22 нФ,тип 1206','2009','2011','22','0','0',NULL,NULL,'2008','0','2014-05-26 13:24:51'),(2124,'MDSTBVA 2,5/ 4-G','2300','2032','24','0','0',NULL,NULL,'2008','0','2014-05-26 16:34:47'),(2125,'MDSTBVA 2,5/2-G','2300','2032','25','0','0',NULL,NULL,'2008','0','2014-05-26 16:37:44'),(2126,'231-132','2300','2018','26','0','0',NULL,NULL,'2008','0','2014-05-26 16:42:58'),(2127,'MCV 1,5/ 9-G-3,5','2300','2032','27','0','0',NULL,NULL,'2008','0','2014-05-26 17:38:14'),(2128,'VDX-6357RD-512','2700','2033','1','0','0',NULL,NULL,'2001','0','2014-05-28 15:00:24'),(2129,'MKDS 3/2','2300','2032','28','0','0',NULL,NULL,'2008','0','2014-05-29 14:51:16'),(2130,'MKDS 3/8','2300','2032','29','0','0',NULL,NULL,'2008','0','2014-05-29 15:27:27'),(2131,'Чип Рез. 0,063 Вт-220 Ом-тип 0603','2113','2011','43','0','0',NULL,NULL,'2008','0','2014-05-30 10:39:52'),(2132,'Переключатель угловой ESP-2010','2043','2011','14','0','0',NULL,NULL,'2008','0','2014-05-30 14:55:45'),(2133,'PLY17BN1023R0A2','2700','2010','8','0','0',NULL,NULL,'2008','0','2014-06-04 16:57:37'),(2134,' Конд. 0,33 мкФ,тип  К73-17','2007','2011','23','0','0',NULL,NULL,'2008','0','2014-06-04 17:21:34'),(2135,' Конд. 0,22 мкФ,тип К73-17','2007','2011','24','0','0',NULL,NULL,'2008','0','2014-06-04 17:23:09'),(2136,'B72214S0271K101','2047','2012','10','0','0',NULL,NULL,'2008','0','2014-06-04 17:42:12'),(2137,'MKDS 3/3','2300','2032','30','0','0',NULL,NULL,'2008','0','2014-06-05 11:44:58'),(2138,'Индуктивность EC36-150K','2700','2011','9','0','0',NULL,NULL,'2008','0','2014-06-05 13:36:31'),(2139,'1SMB5949BT3','2050','2013','11','0','0',NULL,NULL,'2008','0','2014-06-10 11:04:00'),(2140,'H11L1SM','2350','2002','10','0','0',NULL,NULL,'2008','0','2014-06-24 14:39:40'),(2141,'74HC4046AD','2400','2001','1','0','0',NULL,NULL,'2008','0','2014-06-27 10:09:12'),(2142,'AD73360AR','2400','2001','1','0','0',NULL,NULL,'2008','0','2014-06-27 11:08:25'),(2143,'TZ77V','2041','2011','1','0','0',NULL,NULL,'2008','0','2014-06-27 12:39:44'),(2144,'TZ111V','2041','2011','6','0','0',NULL,NULL,'2008','0','2014-06-27 12:46:14'),(2145,'SL 5.08HC/06/90LF','2300','2035','31','0','0',NULL,NULL,'2008','0','2014-06-30 16:20:57'),(2146,'769-666/003-000','2300','2018','32','0','0',NULL,NULL,'2008','0','2014-07-02 18:34:12'),(2147,'Вилка на плату PLD-40','2300','2011','33','0','0',NULL,NULL,'2001','0','2014-07-04 16:58:43'),(2148,'Вилка на плату PLD-64','2300','2011','34','0','0',NULL,NULL,'2001','0','2014-07-04 16:59:35'),(2149,'Вилка на плату PLS2-10R','2300','2011','35','0','0',NULL,NULL,'2008','0','2014-07-07 13:14:11'),(2150,'Вилка на плату PBS2-10','2300','2011','36','0','0',NULL,NULL,'2008','0','2014-07-07 13:17:28'),(2151,'SV 7.62HP/06/90MSF','2300','2035','37','0','0',NULL,NULL,'2008','0','2014-07-07 14:56:44'),(2152,'PLS2-8R','2300','2036','38','0','0',NULL,NULL,'2010','0','2014-07-09 14:02:18'),(2153,'PLS2-15R','2300','2036','39','0','0',NULL,NULL,'2010','0','2014-07-09 15:41:59'),(2154,'PBS2-15','2300','2036','40','0','0',NULL,NULL,'2010','0','2014-07-09 15:44:26'),(2155,'Чип Рез. -6,49 кОм-1%,тип 0603','2113','2011','44','0','0',NULL,NULL,'2008','0','2014-07-09 18:48:52'),(2156,'SV 7,62HP/06/90G','2300','2035','41','0','0',NULL,NULL,'2010','0','2014-07-10 17:36:57'),(2157,'Резистор С2-29В-0,125','2110','2037','45','0','0',NULL,NULL,'2010','0','2014-07-11 13:47:16'),(2158,'С2-29В-0,25','2110','2037','46','0','0',NULL,NULL,'2010','0','2014-07-11 13:51:18'),(2159,'770-803/011-000','2300','2018','42','0','0',NULL,NULL,'2001','0','2014-07-14 14:12:41'),(2160,'ISO7640FM','2400','2022','1','0','0',NULL,NULL,'2010','0','2014-07-15 12:43:32'),(2161,'Вилка на плату MF-02MB','2300','2011','43','0','0',NULL,NULL,'2001','0','2014-07-15 12:54:29'),(2162,'Вилка на плату MF-04','2300','2011','44','0','0',NULL,NULL,'2001','0','2014-07-15 12:55:27'),(2163,'6368150','2300','2009','45','0','0',NULL,NULL,'2001','0','2014-07-15 14:07:59'),(2164,'MEF1S2405SP3C','2600','2010','3','0','0',NULL,NULL,'2010','0','2014-07-16 11:27:20'),(2165,'SDR0403-6R8ML','2700','2025','10','0','0',NULL,NULL,'2010','0','2014-07-16 12:06:40'),(2166,'1N4148WS','2050','2002','12','0','0',NULL,NULL,'2008','0','2014-07-17 18:10:57'),(2167,'SV 7,62HP/04/90G','2300','2035','46','0','0',NULL,NULL,'2010','0','2014-07-18 14:12:54'),(2168,'SV 7.62HP/04/90MSF3','2300','2035','47','0','0',NULL,NULL,'2010','0','2014-07-22 11:58:27'),(2169,'BC847A','2080','2002','6','0','0',NULL,NULL,'2001','0','2014-07-22 12:48:29'),(2170,'Чип Конд. 2,2 мкФ,10 В,тип 0603','2009','2011','25','0','0',NULL,NULL,'2008','0','2014-07-22 14:00:32'),(2171,'500075-0517','2300','2038','48','0','0',NULL,NULL,'2008','0','2014-07-23 18:19:54'),(2172,'Гнездо на плату PBS2-5','2300','2036','49','0','0',NULL,NULL,'2010','0','2014-07-24 17:58:08'),(2173,'Вилка угловая на плату PLS2-5R','2300','2036','50','0','0',NULL,NULL,'2010','0','2014-07-24 18:08:49'),(2174,'С2-29В-0,25-1,0 МОм-0,1%-1,0-А','2110','2037','47','0','0',NULL,NULL,'2010','0','2014-07-28 15:00:38'),(2175,'С2-29В-0,125-9,53 кОм-0,1%-1.0-А','2110','2037','48','0','0',NULL,NULL,'2010','0','2014-07-28 15:03:01'),(2176,'AD8554AR','2400','2001','7','0','0',NULL,NULL,'2010','0','2014-07-28 15:27:25'),(2177,'С2-29В-0,125-274 Ом-0,1%-1.0-А','2110','2037','49','0','0',NULL,NULL,'2010','0','2014-07-28 16:32:02'),(2178,'Чип Конд. 0.022 мкФ,тип 0603','3','2011','','','','','','2001','0','27/06/2015 12:20:37'),(2179,'Чип Конд. 0,022 мкФ,тип 0603','2001','2036','1.27','','','','','2001','0','28/06/2015 11:18:33'),(2180,'С2-29В-0,125-249 Ом-0,1%-1.0-А','2110','2037','50','0','0',NULL,NULL,'2010','0','2014-07-28 17:08:12'),(2181,'Плен. Конд. 0,22 мкФ,тип ','2007','2011','28','0','0',NULL,NULL,'2008','0','2014-07-29 18:12:27'),(2182,'Плен. Конд. 0,33 мкФ,тип ','2007','2011','29','0','0',NULL,NULL,'2008','0','2014-07-29 18:13:54'),(2183,'5747462-4','2300','2009','51','0','0',NULL,NULL,'2008','0','2014-07-31 17:14:33'),(1,'999','2041','2009','Элемент в БД altium','Элемент в БД solidworks','Элемент в БД schemagee','Элемент в БД конструктивов','Элемент в БД устройств','2001','0','25/02/2015 21:01:45'),(2,'таблички,корпуса','2700','2003','Элемент в БД altium','Элемент в БД solidworks','Элемент в БД schemagee','Элемент в БД конструктивов','Элемент в БД устройств','2001','0','25/02/2015 21:24:57');
/*!40000 ALTER TABLE `nk` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `nkwh`
--

DROP TABLE IF EXISTS `nkwh`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `nkwh` (
  `idnkwh` int(11) NOT NULL,
  `nkwh` varchar(45) DEFAULT NULL,
  `idnk` varchar(128) DEFAULT NULL,
  `idwh` varchar(128) DEFAULT NULL,
  `rack` varchar(128) DEFAULT NULL,
  `box` varchar(128) DEFAULT NULL,
  `cell` varchar(128) DEFAULT NULL,
  `idpers` varchar(128) DEFAULT NULL,
  `date` varchar(128) DEFAULT NULL,
  `deleted` int(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`idnkwh`),
  UNIQUE KEY `idnkwh_UNIQUE` (`idnkwh`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `nkwh`
--

LOCK TABLES `nkwh` WRITE;
/*!40000 ALTER TABLE `nkwh` DISABLE KEYS */;
INSERT INTO `nkwh` VALUES (1,'pc.ent.nk','pc.ent.nk','pc.ent.wh','','','','','0000-00-00 00:00:00',0),(2,'Элемент','','','Шкаф','Ящик','Ячейка','','0000-00-00 00:00:00',0),(4,'2087','2087','2001','1','1','a1','2001','06/10/2014 09:53:14',0),(3,'2088','2088','3','4','2','d1','2001','06/10/2014 09:53:14',0),(5,'2046','2046','3','1','1','a1','2001','06/10/2014 16:31:12',0);
/*!40000 ALTER TABLE `nkwh` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `objects`
--

DROP TABLE IF EXISTS `objects`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `objects` (
  `idobjects` int(11) NOT NULL,
  `objects` varchar(45) DEFAULT NULL,
  `company` varchar(45) DEFAULT NULL,
  `country` varchar(10) DEFAULT NULL,
  `index` varchar(10) DEFAULT NULL,
  `city` varchar(10) DEFAULT NULL,
  `address` varchar(256) DEFAULT NULL,
  `contact1` varchar(10) DEFAULT NULL,
  `contact2` varchar(10) DEFAULT NULL,
  `contact3` varchar(10) DEFAULT NULL,
  `asu` varchar(10) DEFAULT NULL,
  `installator` varchar(10) DEFAULT NULL,
  `adjuster` varchar(10) DEFAULT NULL,
  `projecter` varchar(10) DEFAULT NULL,
  `equipment` varchar(45) DEFAULT NULL,
  `affiliation` varchar(45) DEFAULT NULL,
  `contract` varchar(128) DEFAULT NULL,
  `contractor` varchar(10) DEFAULT NULL,
  `contdate` varchar(45) DEFAULT NULL,
  `prepay` varchar(45) DEFAULT NULL,
  `startdate` varchar(45) DEFAULT NULL,
  `initdatasend` varchar(45) DEFAULT NULL,
  `initdatarecv` varchar(45) DEFAULT NULL,
  `factory` varchar(10) DEFAULT NULL,
  `factjob` varchar(45) DEFAULT NULL,
  `factdoc` varchar(45) DEFAULT NULL,
  `ordate` varchar(45) DEFAULT NULL,
  `shippingdate` varchar(45) DEFAULT NULL,
  `waybill` varchar(10) DEFAULT NULL,
  `operationdate` varchar(45) DEFAULT NULL,
  `idpers1` varchar(45) DEFAULT NULL,
  `idpers2` varchar(45) DEFAULT NULL,
  `guaranteedate` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idobjects`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `objects`
--

LOCK TABLES `objects` WRITE;
/*!40000 ALTER TABLE `objects` DISABLE KEYS */;
/*!40000 ALTER TABLE `objects` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `parameters`
--

DROP TABLE IF EXISTS `parameters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `parameters` (
  `idparameters` int(11) NOT NULL,
  `parameters` varchar(45) DEFAULT NULL,
  `par1name` varchar(45) DEFAULT NULL,
  `par2name` varchar(45) DEFAULT NULL,
  `par3name` varchar(45) DEFAULT NULL,
  `par4name` varchar(45) DEFAULT NULL,
  `par5name` varchar(45) DEFAULT NULL,
  `par1unitset` varchar(45) DEFAULT NULL,
  `par2unitset` varchar(45) DEFAULT NULL,
  `par3unitset` varchar(45) DEFAULT NULL,
  `par4unitset` varchar(45) DEFAULT NULL,
  `par5unitset` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  PRIMARY KEY (`idparameters`),
  UNIQUE KEY `idparameters_UNIQUE` (`idparameters`)
) ENGINE=MyISAM AUTO_INCREMENT=29 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `parameters`
--

LOCK TABLES `parameters` WRITE;
/*!40000 ALTER TABLE `parameters` DISABLE KEYS */;
INSERT INTO `parameters` VALUES (3,'Конденсаторы','Ёмкость','Ном. напряжение',NULL,NULL,NULL,'2',NULL,NULL,NULL,NULL,0),(4,'Разъемы','Количество контактов','Рабочий ток','Напряжение изоляции',NULL,NULL,'0',NULL,NULL,NULL,NULL,0),(5,'Диоды','Падение напряжения','Рабочий ток','Скорость переключения','Обр. напряжение',NULL,'0',NULL,NULL,NULL,NULL,0),(6,'Управление питанием','Входное напряжение','Выходное напряжение','Рабочий ток','Рабочая частота','Падение напряжения','0',NULL,NULL,NULL,NULL,0),(8,'Изоляторы','Напряжение изоляции','Напряжение питания','Кол-во каналов на стороне 1','Кол-во каналов на стороне 2','Максимальная скорость','0',NULL,NULL,NULL,NULL,0),(9,'Оптоэлектроника','Цвет свечения','Падение напряжения','Количество в корпусе',NULL,NULL,'0',NULL,NULL,NULL,NULL,0),(10,'Источники питания','Входное напряжение','Выходное напряжение','Количество выходов',NULL,NULL,'0',NULL,NULL,NULL,NULL,0),(11,'Резисторы','Сопротивление','Напряжение изоляции',NULL,NULL,NULL,'1',NULL,NULL,NULL,NULL,0),(12,'Транзисторы','Рабочий ток','Обр. напряжение','Коэфф. усиления','Скорость переключения',NULL,'0',NULL,NULL,NULL,NULL,0),(7,'Источники опорного напряжения','Входное напряжение','Выходное напряжение','Выходной ток',NULL,NULL,'0',NULL,NULL,NULL,NULL,0),(16,'Устройства коммутации','Количество в корпусе','Рабочий ток','Напряжение изоляции',NULL,NULL,'0',NULL,NULL,NULL,NULL,0),(17,'Модули','Тип','',NULL,NULL,NULL,'0',NULL,NULL,NULL,NULL,0),(20,'Варисторы','Ном. напряжение RMS','Ном. напряжение DC','Энергия','Ток (8/20)',NULL,'0',NULL,NULL,NULL,NULL,0),(21,'Защитные диоды','Ном. напряжение','Напряжение пробоя','Ток (10/1000)',NULL,NULL,'0',NULL,NULL,NULL,NULL,0),(22,'Ключи аналоговые','Кол-во каналов','Проходное сопротивление','Рабочее напряжение','Скорость переключения','Тип','0',NULL,NULL,NULL,NULL,0),(23,'Оптроны','Падение напряжения','Макс. обр. напряжение','Скорость переключения','Коэффициент передачи по току','Тип','0',NULL,NULL,NULL,NULL,0),(24,'Интерфейсы','Скорость передачи','Тип','Количество каналов',NULL,NULL,'0',NULL,NULL,NULL,NULL,0),(15,'Операционные усилители','Рабочее напряжение','Скорость нарастания','Коэфф. усиления','Напряжение смещения','КОСС','0',NULL,NULL,NULL,NULL,0),(13,'Защита','Кол-во каналов','Ном. напряжение','Применение',NULL,NULL,'0',NULL,NULL,NULL,NULL,0),(25,'Индуктивности','Индуктивность','Рабочий ток','Добротность','Сопротивление',NULL,'0',NULL,NULL,NULL,NULL,0),(26,'Микроконтроллеры','Тактовая частота','Архитектура','ОЗУ','ПЗУ','Напряжение питания','0',NULL,NULL,NULL,NULL,0),(27,'Память','Ёмкость','Ном. напряжение','Скорость записи','Скорость чтения','Тип','0',NULL,NULL,NULL,NULL,0),(28,'Генераторы','Частота','Тип','Ёмкость нагрузки','Напряжение питания',NULL,'0',NULL,NULL,NULL,NULL,0),(30,'Позисторы','Рабочий ток','Сопр. в открытом состоянии','Время срабатывания','Рабочее напряжение',NULL,'0',NULL,NULL,NULL,NULL,0),(29,'Разрядники','Ном. напряжение','Напряжение горения дуги',NULL,NULL,NULL,'0',NULL,NULL,NULL,NULL,0),(1,'Набор параметров','Параметр 1','Параметр 2','Параметр 3','Параметр 4','Параметр 5','pc.sup.unitset','pc.sup.unitset','pc.sup.unitset','pc.sup.unitset','pc.sup.unitset',0),(2,'Наименование набора параметров','Наименование 1-го параметра','Наименование 2-го параметра','Наименование 3-го параметра','Наименование 4-го параметра','Наименование 5-го параметра','Единицы измерения 1-го параметра','Единицы измерения 2-го параметра','Единицы измерения 3-го параметра','Единицы измерения 4-го параметра','Единицы измерения 5-го параметра',0),(35,'Аналоговые (разное)','Описание','Ном. напряжение','Параметр 3','Параметр 4','Параметр 5','0',NULL,NULL,NULL,NULL,0),(36,'АЦП и ЦАП','Разрешение (бит)','Скорость оцифровки','Опорное напряжение','Количество каналов','Тип цифрового выхода','0',NULL,NULL,NULL,NULL,0),(37,'Трансформаторы','Коэффициент трансформации','Максимальный первичный ток/напряжение','Сопротивление обмотки','Точность по амплитуде','Точность по углу','0',NULL,NULL,NULL,NULL,0),(38,'Изоляторы','Напряжение изоляции','Напряжение питания','Кол-во каналов на стороне 1','Кол-во каналов на стороне 2','Максимальная скорость','0',NULL,NULL,NULL,NULL,0);
/*!40000 ALTER TABLE `parameters` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `periodic`
--

DROP TABLE IF EXISTS `periodic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `periodic` (
  `idperiodic` int(11) NOT NULL,
  `periodic` varchar(128) DEFAULT NULL,
  `value` varchar(128) DEFAULT NULL,
  `date` datetime DEFAULT NULL,
  `deleted` int(1) NOT NULL DEFAULT '0',
  `idpers` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`idperiodic`),
  UNIQUE KEY `idperiodic_UNIQUE` (`idperiodic`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `periodic`
--

LOCK TABLES `periodic` WRITE;
/*!40000 ALTER TABLE `periodic` DISABLE KEYS */;
INSERT INTO `periodic` VALUES (1,'Наименование','Значение','0000-00-00 00:00:00',0,NULL),(2,'Наименование','Значение','0000-00-00 00:00:00',0,NULL),(3,'НДС','18',NULL,0,NULL),(4,'RUB','1',NULL,0,'2001'),(5,'USD','41.0501',NULL,0,'2001'),(6,'EUR','52.6468',NULL,0,'2001');
/*!40000 ALTER TABLE `periodic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `qaltium`
--

DROP TABLE IF EXISTS `qaltium`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qaltium` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tablename` varchar(45) DEFAULT NULL,
  `Library Ref` varchar(45) DEFAULT NULL,
  `Library Path` varchar(256) DEFAULT NULL,
  `Footprint Ref` varchar(45) DEFAULT NULL,
  `Footprint Path` varchar(256) DEFAULT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) DEFAULT NULL,
  `PartNumber` varchar(64) DEFAULT NULL,
  `Package` varchar(45) DEFAULT NULL,
  `Marking` varchar(45) DEFAULT NULL,
  `NominalValue` varchar(45) DEFAULT NULL,
  `NominalVoltage` varchar(45) DEFAULT NULL,
  `Tolerance` varchar(45) DEFAULT NULL,
  `OpTemperaturen` varchar(45) DEFAULT NULL,
  `OpTemperaturem` varchar(45) DEFAULT NULL,
  `Pmax` varchar(45) DEFAULT NULL,
  `TC` varchar(45) DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` varchar(45) DEFAULT NULL,
  `Discontinued` varchar(45) DEFAULT NULL,
  `Description` varchar(64) DEFAULT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  `prefix` varchar(45) DEFAULT NULL,
  `isSMD` varchar(45) DEFAULT NULL,
  `Nominal` varchar(45) DEFAULT NULL,
  `Unit` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=21 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `qaltium`
--

LOCK TABLES `qaltium` WRITE;
/*!40000 ALTER TABLE `qaltium` DISABLE KEYS */;
/*!40000 ALTER TABLE `qaltium` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `qnk`
--

DROP TABLE IF EXISTS `qnk`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qnk` (
  `idqnk` int(11) NOT NULL,
  `qnk` varchar(128) DEFAULT NULL,
  `idclasses` varchar(128) DEFAULT NULL,
  `idmanuf` varchar(128) DEFAULT NULL,
  `idalt` varchar(128) DEFAULT NULL,
  `idsdw` varchar(128) DEFAULT NULL,
  `idsge` varchar(128) DEFAULT NULL,
  `idctr` varchar(128) DEFAULT NULL,
  `iddev` varchar(128) DEFAULT NULL,
  `idpers` varchar(128) DEFAULT NULL,
  `date` varchar(128) DEFAULT NULL,
  `deleted` int(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`idqnk`),
  UNIQUE KEY `idqnk_UNIQUE` (`idqnk`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `qnk`
--

LOCK TABLES `qnk` WRITE;
/*!40000 ALTER TABLE `qnk` DISABLE KEYS */;
INSERT INTO `qnk` VALUES (3,'таблички,корпуса','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,1),(4,'табличка НКВВ','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(5,'корпус НКВВ','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(6,'этикетка АВМ-В','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(7,'корпус АВМ-В','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(8,'диски ','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(9,'книженция','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(10,'табличка АВМ-ВК','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(11,'корпус АВМ-ВК','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(12,'УЗ 485','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(13,'платы','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(14,'УПО2 (прямоуг-180 шт + 60 кругл по 2 шт)','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(15,'PKI_03','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(16,'MOAS_03','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(17,'MRSTN_03','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(18,'PFIZ_003_v03','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(19,'PKIPI_03','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(20,'большая(АВМР.426419.010 ПП АВМ-В вер.20)','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(21,'дополнительная(АВМР.426419.011 ПП АВМ-В вер.4)','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(22,'клавиатуры(АВМР.426419.013 ПП АВМ-В вер.4)','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(23,'блока питания(АВМР.426419.012 ПП АВМ-В вер.3)','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(24,'АСНЛ.687281.016-01 (DT3T)','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(25,'DT3H(АВМР.687281.025 ПП АВМ-В вер.3)','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(26,'AVM_VK','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(27,'','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(28,'','плата',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(29,'WAGO','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(30,'WAGO 2002-2202','клема 2-уровневая',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(31,'WAGO 2002-2292','пластина',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(32,'WAGO 2002-3201','клема 3-уровневая',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(33,'WAGO 2002-3292','пластина',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(34,'WAGO 2002-408','перемычка изолированая',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(35,'WAGO 2002-410','перемычка изолированая',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(36,'WAGO 209-114','защитная крышка                                            ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(37,'WAGO 210-331/500-103','самоклеющаяся маркировка',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(38,'WAGO 210-332/350-202','самоклеющаяся маркировка',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(39,'WAGO 231-104/026-000/033-000','разъем розетка',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(40,'WAGO 231-106/026-000/034-000','разъем розетка',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(41,'WAGO 231-108/026-000/034-000','разъем розетка',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(42,'WAGO 231-111/026-000/035-000','разъем розетка',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(43,'WAGO 231-433/001-000','вилка на плату',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(44,'WAGO 231-434/001-000','вилка на плату',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(45,'WAGO 231-436/001-000','вилка на плату ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(46,'WAGO 231-438/001-000 ','вилка на плату ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(47,'WAGO 231-441/001-000','вилка на плату ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(48,'WAGO 232-203/026-000','разъем розетки',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(49,'WAGO 232-204/026-000','разъем розетки',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(50,'WAGO 232-334','вилка на плату ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(51,'WAGO 232-336','вилка на плату ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(52,'WAGO 232-338','вилка на плату ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(53,'WAGO 249-116','стопор оконечный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(54,'WAGO 249-117','стопор оконечный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(55,'WAGO 249-119','держатель маркировки                                   ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(56,'WAGO 256-402 (ДТ3Х-3 шт.на компл)','клемма колодка 2-кон.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(57,'WAGO 256-403','клемма колодка 3-кон.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(58,'WAGO 256-406 (DT3T - 1шт.)(ДТ3Х-3 шт.на компл)','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(59,'WAGO 272-681  (272-131)','разъем  ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(60,'WAGO 280-309','боковина',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(61,'WAGO 280-315','боковина',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(62,'WAGO 280-326','боковина',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(63,'WAGO 280-633','клемма',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(64,'WAGO 280-677','клемма',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(65,'WAGO 280-681','клемма',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(66,'WAGO 280-685','клемма',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(67,'WAGO 280-687','клемма',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(68,'WAGO 280-901','клемма',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(69,'WAGO 280-907','клемма',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(70,'WAGO 281-326','боковина',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(71,'WAGO 281-335','пластина',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(72,'WAGO 281-652','клема проходная',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(73,'WAGO 281-673/281-400','клемма',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(74,'WAGO 734-104','розетка соединитель',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(75,'WAGO 734-106','розетка соединитель',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(76,'WAGO 734-108','розетка соединитель',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(77,'WAGO 734-110','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(78,'WAGO 734-112','розетка соединитель',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(79,'WAGO 734-130','штифт кодирующий верх',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(80,'WAGO 734-166','вилка угловая под пайку',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(81,'WAGO 734-400','штифт кодирующий низ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(82,'WAGO 734-404/001-000','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(83,'WAGO 734-406','вилка двухуровневая угловая под пайку ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(84,'WAGO 734-406/001-000','вилка двухуровневая угловая под пайку с доп.опорой',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(85,'WAGO 734-408/001-000','вилка двухуровневая угловая под пайку с доп.опорой',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(86,'WAGO 734-410/001-000','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(87,'WAGO 734-412/001-000','вилка двухуровневая угловая под пайку с доп.опорой',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(88,'WAGO 739-106','колодка для плат',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(89,'WAGO 739-124','колодка для плат',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(90,'WAGO 742-100','боковина',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(91,'WAGO 742-121','клемма для печатных плат',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(92,'WAGO 742-124','клемма для печатных плат',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(93,'WAGO 742-128','клемма для печатных плат',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(94,'WAGO 745-102','никуда не идет!!!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(95,'WAGO 769-103/021-000','штекер розетка',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(96,'WAGO 769-411','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(97,'WAGO 769-603/005-000','котактная планка',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(98,'WAGO 793-502','маркировка WMB MULTI гор.(1..10(10х))',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(99,'WAGO 793-503','маркировка WMB MULTI гор.(11..20(10х))',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(100,'WAGO 793-504','маркировка WMB MULTI гор.(21..30(10х))',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(101,'WAGO 793-505','маркировка WMB MULTI гор.(31..40(10х))',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(102,'WAGO WSB 793-5501','вкладыш маркировочный «пустой х 10» ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(103,'WAGO WSB 793-5502','вкладыш маркировочный «1-10 х 10» ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(104,'WAGO WSB 793-5503','вкладыш маркировочный «11-20 х 10» ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(105,'WAGO WSB 793-5602','вкладыш маркировочный «1-10 х 10» ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(106,'WAGO ','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(107,'WAGO ','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(108,'WAGO ','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(109,'WAGO ','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(110,'WAGO ','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(111,'WAGO ','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(112,'микросхемы','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(113,'74AC123(74ALS123)(74HC123soic so-16)(ЭКФ 1533АГЗ)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(114,'74AC132','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(115,'74AC14M (74HC14D; 74HC14DR2G)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(116,'74AC273 (SO-20)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(117,'74AC74','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(118,'74HC138M (74HC138DR2G)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(119,'74HC4046A','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(120,'74LCX541WM(74LVC541AD.112)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(121,'AD5547BRU','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(122,'AD620AR','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(123,'AD623AR','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(124,'AD629AR','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(125,'AD7656BSTZ','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(126,'AD7705BR','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(127,'AD7927BRU','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(128,'AD820AR','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(129,'AD8561AR','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(130,'AD8609AR (soic so-14)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(131,'AD8627ARZ','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(132,'AD8628ART','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(133,'ADG1206YRUZ','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(134,'ADG711BR','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(135,'ADG774BR','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(136,'ADM2483BRW','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(137,'ADM2484EBRWZ','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(138,'ADM3251E','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(139,'ADR291ER','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(140,'ADUM4160BRWZ','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(141,'AM1D-0505SH30 (P6LU-0505ELF)(AM2D-0505SH30 можно на АВМ-ВК)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(142,'AM1D-2405SH30-NZ (P6LU-2405ELF)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(143,'AM1L-2405D-NZ(PSD-2405ZLF)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(144,'AM1L-2405S-NZ(PSD-2405ELF)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(145,'AM29F016D-90EF','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(146,'AM29F016D-90FI','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(147,'AM2D-0524SH30 (P10LU-0524ELF)(AM1D-0524SH30 можно на АВМ-ВК)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(148,'AM2LV-2405D-NZ (TES 2N-2421)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(149,'AM6T-2415SH35Z (PEN6-2415E4:1LF; TEN 5-2413)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(150,'AM6T-2424SH35Z(P26TG-4824E4:1H35LF, VD-2424S6H,  TEN 6-2415WIN-HI)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(151,'AM8T-2403SIZ (P34TG-243R3E2:1MLF)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(152,'AM8T-2415DIZ','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(153,'AMEL5-5SMAZ','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(154,'AMSR1-783.3-NZ','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(155,'AS7C164-20JC (-12JCN)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(156,'AT26DF321SU(AT25DF321A-SH)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(157,'AT29C512-70(90) JI(SST39SF010A-55-4C-NHE)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(158,'AT45DB642D-TU','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(159,'ATF1508AS-10 QI100','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(160,'CSNX25','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(161,'DG401CY(DYZ)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(162,'DG408DY','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(163,'DG409DY','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(164,'DS3231SN','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(165,'EPM7128SQC160-15N','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(166,'FM16W08-SG','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(167,'FM25H20-DG','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(168,'FOD060L (HCPL060L)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(169,'HCPL-2211','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(170,'IL485E','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(171,'IS61LV2568L-10T (CY7C1010DV33-10ZSXI)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(172,'L78S05CV','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(173,'LM1085IT-3.3','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(174,'LM7805 (MC7805B)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(175,'LM7905 (L7905CV TO-220, MC7905B)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(176,'LM79L05 (TO-92-3pins)(L79L05)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(177,'MAX4534_SD','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(178,'MAX4535 SD','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(179,'MAX705ESA  (CSA)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(180,'NC7SZ14 (SN74LVC1G14DBV)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(181,'OP1177AR','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(182,'OP2177AR','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(183,'OP4177AR','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(184,'PEN 4-2412Z4:1LF (TEN 3-2422WI; RDD05-15D1)','преобразователь',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(185,'PGA207UA','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(186,'R-785.0-0.5','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(187,'REF192*S','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(188,'RT424024','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(189,'RTC72423A','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(190,'SG-8002JF(CA+)    29,4912М-PHC ','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(191,'SP232AEN(MAX232AESE;MAX202EESE)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(192,'ST10F269Z2Q3','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(193,'STM32F103ZCT6','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(194,'STM32F407IG','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(195,'TEN3-0522','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(196,'TLP180(PS2705-1L-F3-A)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(197,'TLP281','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(198,'TMA0505S','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(199,'ULN2003ADR','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(200,'ULN2003D','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(201,'USBLC6-2SC6','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(202,'XR16C2852CJ (IJ)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(203,'AM1D-2424SH30','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(204,'TEN8-2423','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(205,'ADM811 (SOT-143)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(206,'LM3480IM3-3.3 (SOT23-3)','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(207,'','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(208,'','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(209,'','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(210,'','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(211,'','микросхема',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(212,'электронные компоненты','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(213,'FNR-14K681(TVR-14681) (PFIZ - 2 шт.)','варистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(214,'S05K150','варистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(215,'S14K275 (FNR-14K431) (PFIZ - 2 шт.)','варистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(216,'S10K11(TVR10180,FNR-10K180,JVR-10N180)','варистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(217,'S20K14, 14 B','варистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(218,'LA25P','датчик тока',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(219,'КД521А (1N4148 = КД522А)','диод',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(220,'1.5KE11CA(12CA)','диод защитный ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(221,'1.5KE12A','диод защитный ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(222,'1.5KE350CA','диод защитный ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(223,'P4KE12A(P6KE12A)','диод защитный ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(224,'P4KE12CA(P6KE12CA)','диод защитный ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(225,'P4KE6,8CA (DT3T - 3шт.)','диод защитный ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(226,'P6KE33CA','диод защитный ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(227,'P6KE91CA','диод защитный ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(228,'SP0503BAHT','диод ограничительный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(229,'MBR0540T1','диод шотки',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(230,'12CTQ045S','диодная сборка  ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(231,'BAT54','диодная сборка  ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(232,'BAT54C','диодная сборка  ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(233,'BAT54S','диодная сборка  ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(234,'BAV99 (ДТ3Х-3шт.на компл)','диодная сборка  ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(235,'DL4448','диодная сборка  ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(236,'PLY10AN1130R5D2B (PFIZ - 1 шт.)','дросель ЭМС',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(237,'HCM1206X','излучатель звука ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(238,'HPS16A','излучатель звука ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(239,'ЗП-18','излучатель звука ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(240,'EECS5R5H105  1Фх5,5В (гор.)','ионистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(241,'EECS5R5V105  1Фх5,5В (верт.)','ионистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(242,'EC36-150K (EC36-120K) 15 мкГн(LGA0410 120 uH K-220 uH K)','катушка индуктивности ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(243,'KX-38 32,768kHz 6pF(DT-38T 32,768kHg)','кварцевый резонатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(244,'KX-3HE 8,0 МГц(НС-49/U 8,0 МГц)','кварцевый резонатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(245,'KX-3HT 24,0 MHZ(HC49S2 24 МГц )','кварцевый резонатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(246,'К50-35-35В-100 мкФ (8 Х 11)','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(247,'К73-17-250В-0,010 мкФ-5%(0,015 мкф??-40 шт.)','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(248,'К73-17-250В-0,022 мкФ-5%','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(249,'К73-17-250В-0,033 мкФ-5%','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(250,'К73-17-250В-0,047 мкФ-5%','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(251,'К73-17-250В-0,060 мкФ-5%','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(252,'К73-17-250В-0,15 мкФ-5% ','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(253,'К73-17-250В-0,33 мкФ-5%   ','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(254,'К73-17-250В-1,0 мкФ-5%','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(255,'К73-17-250В-2,2 мкФ-5%   ','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(256,'К73-17-250В-4,7 мкФ-5% ','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(257,'К73-17-250В-6,8 мкФ-10% ','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(258,'К73-17-400В-0,22 мкФ-10% (PFIZ - 1 шт.)','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(259,'К73-17-630В-0,33 мкФ-10% (PFIZ - 2 шт.)','конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(260,'B32922С3684K(M),  0,68 мкФ','конденсатор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(261,'3302X-3-103 10 кОм','подстроечн резистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(262,'СП3-19А-10кОм ± 10%(3329H-1-103LF,PV32H-10kOm)','подстроечн резистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(263,'A81-A600X(M51-A600; N81-A600)','разрядник ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(264,'EC75X,  75B','разрядник ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(265,'EC90','разрядник ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(266,'T81-A90X','разрядник ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(267,'T83-A250(230)X или B88069X8340B502 EPCOS','разрядник ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(268,'T83-A90X(B88069X8300B502)','разрядник ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(269,'C2-23-0,5-1 МOм±5%-A-B-B','резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(270,'C2-23-0,5-100 Oм±10%-A-B-B','резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(271,'C2-23-0,5-130 кOм±5%-A-B-B','резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(272,'C2-23-0,5-22 кOм±5%-A-B-B','резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(273,'C2-23-0,5-270 кOм±5%-A-B-B','резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(274,'C2-23-0,5-33 кOм±5%-A-B-B','резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(275,'C2-23-0,5-43 кOм±5%-A-B-B','резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(276,'C2-23-0,5-510 кOм±5%-A-B-B','резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(277,'C2-23-0,5-68 кOм±5%-A-B-B','резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(278,'C2-23-2-10 Oм±5%-A-B-B(С2-33Н)','резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(279,'C2-23-2-12 кОм±5%','резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(280,'C2-29B-0,25-100 кОм±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(281,'C2-29B-0,25-100 Ом±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(282,'C2-29B-0,25-12,4 кОм±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(283,'C2-29В-0,125-10 Oм±0,5%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(284,'C2-29В-0,125-100 Oм±0,5%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(285,'C2-29В-0,125-12,4 Oм±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(286,'C2-29В-0,125-249 Ом±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(287,'C2-29В-0,125-909 Ом±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(288,'C2-29В-0,25-12,4 кOм±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(289,'C2-29В-0.25-10 кОм±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(290,'C2-29В-0.25-10 Ом±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(291,'C2-29В-0.25-100 кОм±0.1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(292,'C2-29В-0.25-100 Ом±0,25%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(293,'C2-29В-0.25-124 Ом±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(294,'C2-29В-0.25-160 Ом±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(295,'C2-29В-0.25-2 кОм±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(296,'C2-29В-0.25-22,6 кОм±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(297,'C2-29В-0.25-30 Ом±0,25% (30,1 Ом)','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(298,'C2-29В-0.25-4,81 кОм±0,1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(299,'C2-29В-0.25-68.1 кОм±0.1%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(300,'C2-29В-0.5-249 Ом±0.5%','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(301,'C2-29В-2-10 Ом±0,5%-1-Б (DT3T - 3шт.)','резистор точный',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(302,'HP-1-4-9M 1.0кОм','резисторная сборка',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(303,'HP-1-4-9M 4.7кОм','резисторная сборка',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(304,'IM07TS','реле',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(305,'RT424005  (5-1393243-9)','реле',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(306,'MF-R010','самовосст.предохранитель',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(307,'MF-R020','самовосст.предохранитель',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(308,'MF-R040','самовосст.предохранитель',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(309,'MF-USMF005','самовосст.предохранитель',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(310,'MF-USMF010 0,10A','самовосст.предохранитель',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(311,'L-1154IT','светодиод  ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(312,'L53IT','светодиод  ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(313,'L-53LGD','светодиод  ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(314,'L-53LID','светодиод  ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(315,'L-53LYD','светодиод  ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(316,'KB-2620EW кр','светодиодные линейка',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(317,'KB-2700YW жел.','светодиодные линейка',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(318,'KB-2820SGD зел.','светодиодные линейка',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(319,'BZX55C2V7 (BZX79-C2V7)','стабилитрон ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(320,'BZX55C3V9','стабилитрон ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(321,'BZX84C2V7','стабилитрон ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(322,'BZX84C8V2','стабилитрон ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(323,'HEL-776-A-T-1(700-101BAA-B00 honey)','термистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(324,'2455R-01000084(ордер-код 1082187)','термопереключатель',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(325,'TZV-086 2A/250V(ZH103-96)','термопредохранитель ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(326,'Р1-8М 0,125 Вт-1,5 кОм±0,25%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(327,'Р1-8М 0,125 Вт-2,49 кОм±0,25%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(328,'Р1-8М 0,125 Вт-24 Ом±0,25%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(329,'Р1-8М 0,125 Вт-680 Ом±0,25%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(330,'Р1-8М 0,25 Вт-160 Ом±0,25%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(331,'Р1-8М 0,25 Вт-4.12 кОм±0,25%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(332,'Р1-8МП 0,25 Вт-10 Ом±0,25%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(333,'Р1-8МП 0,25 Вт-11,3 кОм±1%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(334,'Р1-8МП 0,25 Вт-140 Ом±0,25%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(335,'Р1-8МП 0,25 Вт-160 Ом±0,25%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(336,'Р1-8МП 0,25 Вт-24 Ом±0,5%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(337,'Р1-8МП 0,25 Вт-3,92 кОм±0,25%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(338,'Р1-8МП 0,25 Вт-4,12 кОм±1%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(339,'Р1-8МП 0,25 Вт-4,64 кОм±0,25%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(340,'Р1-8МП 0,25 Вт-499 Ом±0,1%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(341,'Р1-8МП 0,25 Вт-5,62 кОм±1%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(342,'Р1-8МП 0,25 Вт-6,98 кОм±0,5%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(343,'Р1-8МП 0,25 Вт-7,87 кОм±0,25%,1206(ткс-25)','точный чип-резистор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(344,'BC547A(КТ3102АМ)','транзистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(345,'BC848ALT1','транзистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(346,'BC857ALT1','транзистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(347,'BC857CLT1','транзистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(348,'BCP68T1 (BCP68.115)','транзистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(349,'BCP69T1','транзистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(350,'IRF4905','транзистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(351,'IRF7433','транзистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(352,'BC556A(КТ3107А)','транзистор(DIOTEC)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(353,'AC1005','трансформатор тока',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(354,'AC1075(1060) (DT3T - 3шт.)','трансформатор тока',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(355,'CN1210K11G','чип-варистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(356,'CN1210K30G','чип-варистор ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(357,'CM453232, 10 мкГ, 1812','чип-индуктивность ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(358,'CM453232, 560мкГ, 1812(NL453232-516J-PF(TDK))','чип-индуктивность ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(359,'SMD 0805 0,022 мкф','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(360,'SMD 0805 0,047 мкф','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(361,'SMD 0805 0,1 мкФ','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(362,'SMD 0805 10 пФ','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(363,'SMD 0805 100 пФ','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(364,'SMD 0805 4700 пФ','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(365,'SMD 1206 0,047 мкФ','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(366,'SMD 1206 0,1 мкФ(1 шт. в Элару)(ДТ3Х-18шт.на компл)','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(367,'SMD 1206 0,15 мкФ','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(368,'SMD 1206 0,33 мкФ','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(369,'SMD 1206 1 мкФ','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(370,'SMD 1206 10 нФ ±10%','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(371,'SMD 1206 100 пФ±5%','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(372,'SMD 1206 1000 пФ (1 нФ)','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(373,'SMD 1206 120 пФ','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(374,'SMD 1206 130 пФ','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(375,'SMD 1206 2.2 нФ ±10%','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(376,'SMD 1206 27 нФ ±10%','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(377,'SMD 1206 4700 пФ','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(378,'SMD 1206 5,1 пФ ','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(379,'SMD 1206 560 пФ ±10%','чип-конденсатор',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(380,'10 мкФ х 16В тип С, 10%','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(381,'10 мкФ х 35В тип D, 10% (ДТ3Х-9шт.на компл))','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(382,'10 мкФ х 6,3В тип А ±20%','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(383,'10 мкФ х 6,3В тип С, 10%(10В)','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(384,'2,2 мкФ х 6,3В тип А ±20%','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(385,'2,2 мкФ х 6,3В тип С, 10%(25В)','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(386,'22 мкФ х 10В тип С, 10%','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(387,'22 мкФ х 16В тип С, 10%','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(388,'22 мкФ х 35В тип D, 10%','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(389,'22 мкФ х 6,3В тип А ±20%','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(390,'22 мкФ х 6,3В тип С, 10%(1 шт. в Элару)','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(391,'47 мкФ х 10В тип В','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(392,'47 мкФ х 25В тип Е','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(393,'6,8 мкФ х 20В тип С, 10%','чип-конденсатор тантал.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(394,'YC324  4 x 1 кОм  (1206х4)','чип-резистор матрица',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(395,'YC324  4 x 120 Ом (0603х4)','чип-резистор матрица',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(396,'YC324  4 x 120(100) Ом (1206х4)','чип-резистор матрица',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(397,'YC324  4 x 4,7  кОм (0603х4)','чип-резистор матрица',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(398,'YC324  4 x 4,7  кОм (1206х4)','чип-резистор матрица',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(399,'YC324  4 x 7,5  кОм (0603х4)','чип-резистор матрица',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(400,'YC324  4 x 7,5(6,8)  кОм (1206х4)','чип-резистор матрица',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(401,'SMD 0805 0 Ом±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(402,'SMD 0805 1 кОм±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(403,'SMD 0805 1 МОм±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(404,'SMD 0805 1.1 кОм±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(405,'SMD 0805 1.2 кОм±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(406,'SMD 0805 1.8 кОм±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(407,'SMD 0805 10 кОм±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(408,'SMD 0805 12 кОм±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(409,'SMD 0805 120 Ом±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(410,'SMD 0805 15 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(411,'SMD 0805 16 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(412,'SMD 0805 180 Ом±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(413,'SMD 0805 2,4 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(414,'SMD 0805 2.7 кОм±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(415,'SMD 0805 200 Ом±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(416,'SMD 0805 3 кОм±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(417,'SMD 0805 33 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(418,'SMD 0805 330 Ом±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(419,'SMD 0805 390 Ом±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(420,'SMD 0805 4.7 кОм±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(421,'SMD 0805 47 кОм±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(422,'SMD 0805 680 Ом±10%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(423,'SMD 0805 910 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(424,'SMD 1206 0 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(425,'SMD 1206 1 кОм±1%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(426,'SMD 1206 1 кОм±5%(7 шт. в Элару)','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(427,'SMD 1206 1 МОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(428,'SMD 1206 1,1 МОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(429,'SMD 1206 1,2 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(430,'SMD 1206 1,2 МОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(431,'SMD 1206 1,5 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(432,'SMD 1206 1,8 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(433,'SMD 1206 10 кОм±5%(9 шт. в Элару)(10 шт.плата клавиатуры)','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(434,'SMD 1206 10 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(435,'SMD 1206 100 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(436,'SMD 1206 100 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(437,'SMD 1206 12 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(438,'SMD 1206 12 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(439,'SMD 1206 120 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(440,'SMD 1206 120 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(441,'SMD 1206 150 Ом±5%(плата клавиатуры)','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(442,'SMD 1206 180 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(443,'SMD 1206 180 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(444,'SMD 1206 2,4 кОм±5%(1 шт. в Элару)','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(445,'SMD 1206 20 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(446,'SMD 1206 200 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(447,'SMD 1206 24 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(448,'SMD 1206 24 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(449,'SMD 1206 240 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(450,'SMD 1206 240 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(451,'SMD 1206 270 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(452,'SMD 1206 3,0 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(453,'SMD 1206 3,3 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(454,'SMD 1206 3,9 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(455,'SMD 1206 33 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(456,'SMD 1206 330 Ом±5%(11 шт. в Элару)','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(457,'SMD 1206 36 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(458,'SMD 1206 390 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(459,'SMD 1206 4,3 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(460,'SMD 1206 4,7 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(461,'SMD 1206 470 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(462,'SMD 1206 485 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(463,'SMD 1206 5,0 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(464,'SMD 1206 5,6 МОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(465,'SMD 1206 50 Ом±5%(40-60 Ом)','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(466,'SMD 1206 560 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(467,'SMD 1206 620 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(468,'SMD 1206 68 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(469,'SMD 1206 7,5 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(470,'SMD 1206 820 Ом±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(471,'SMD 1206 9,1 кОм±5%','чип-резистор(SMD)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(472,'NM7010B_SPI','',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(473,'SS24 (DO-214АА_SMB)','диод',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0),(474,'BZX55C5V6','стабилитрон ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
/*!40000 ALTER TABLE `qnk` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `qschemagee`
--

DROP TABLE IF EXISTS `qschemagee`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qschemagee` (
  `idqschemagee` int(11) NOT NULL,
  `qschemagee` varchar(45) DEFAULT NULL,
  `idclasses` varchar(45) DEFAULT NULL,
  `prefix` varchar(128) DEFAULT NULL,
  `type` varchar(256) DEFAULT NULL,
  `catnum` varchar(45) DEFAULT NULL,
  `decnum` varchar(45) DEFAULT NULL,
  `revnum` varchar(45) DEFAULT NULL,
  `idmanuf` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idqschemagee`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `qschemagee`
--

LOCK TABLES `qschemagee` WRITE;
/*!40000 ALTER TABLE `qschemagee` DISABLE KEYS */;
/*!40000 ALTER TABLE `qschemagee` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `reasons`
--

DROP TABLE IF EXISTS `reasons`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `reasons` (
  `idreasons` int(11) NOT NULL,
  `reasons` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  `date` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idreasons`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `reasons`
--

LOCK TABLES `reasons` WRITE;
/*!40000 ALTER TABLE `reasons` DISABLE KEYS */;
INSERT INTO `reasons` VALUES (1,'Основание',0,NULL,NULL),(2,'Основание для отгрузки или приёмки',0,NULL,NULL),(3,'Заполнение',1,NULL,NULL),(4,'Ввод остатков',0,NULL,NULL),(5,'Приём в ремонт',0,NULL,'2001'),(6,'Изготовление',1,NULL,NULL),(7,'Изготовление',0,NULL,NULL),(8,'Из ремонта',0,NULL,'2001'),(9,'Закупка',0,NULL,'2001');
/*!40000 ALTER TABLE `reasons` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `unitset`
--

DROP TABLE IF EXISTS `unitset`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `unitset` (
  `idunitset` int(11) NOT NULL AUTO_INCREMENT,
  `alias` varchar(45) DEFAULT NULL,
  `idalias` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  PRIMARY KEY (`idunitset`)
) ENGINE=MyISAM AUTO_INCREMENT=19 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `unitset`
--

LOCK TABLES `unitset` WRITE;
/*!40000 ALTER TABLE `unitset` DISABLE KEYS */;
INSERT INTO `unitset` VALUES (3,'Ом','9',1),(4,'кОм','9',1),(5,'МОм','9',1),(6,'ГОм','9',1),(7,'пФ','10',0),(8,'мкФ','10',0),(1,'Единицы','pc.ent.unitset',0),(2,'Единица измерения','Набор единиц измерения',0),(9,'Сопротивление','0',1),(10,'Ёмкость','0',0),(11,'Номенклатура','0',0),(12,'шт.','11',0),(18,'м.','11',0);
/*!40000 ALTER TABLE `unitset` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `wh`
--

DROP TABLE IF EXISTS `wh`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wh` (
  `idwh` int(11) NOT NULL,
  `wh` varchar(45) DEFAULT NULL,
  `where` varchar(45) DEFAULT NULL,
  `resperson` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  `date` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idwh`),
  UNIQUE KEY `idwhs_UNIQUE` (`idwh`)
) ENGINE=MyISAM AUTO_INCREMENT=2002 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `wh`
--

LOCK TABLES `wh` WRITE;
/*!40000 ALTER TABLE `wh` DISABLE KEYS */;
INSERT INTO `wh` VALUES (2001,'О203','стр. 9, комн. 203','2001',0,NULL,'2001'),(1,'Наименование','Местонахождение','pc.sup.personel',0,NULL,NULL),(2,'Наименование','Местонахождение','Ответственный',0,NULL,NULL),(3,'Ц6','Стр. 20, комн. 6','3',0,NULL,'2001');
/*!40000 ALTER TABLE `wh` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-06-28 13:20:50
