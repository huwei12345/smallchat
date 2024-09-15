#include "MysqlManager.h"
// 此文件没用到，但是可以使用，在项目还没有数据库的时候初始化数据库

CMysqlManager::CMysqlManager(void)
{
	//TODO: m_strCharactSet���Է��ڳ�ʼ���б��г�ʼ��
    m_strCharactSet = "utf8";
	// ��ʼ���� 
	// 1. t_user 
	STableInfo info;
	//name type desc?
    info.m_strName = "blog";//����;
    info.m_mapField["LinkID"] = { "LinkID", "INT NOT NULL  COMMENT 'LinkID'", "INT" };
	info.m_mapField["record_size"] = { "record_size", " SMALLINT UNSIGNED NOT NULL COMMENT 'recore length'", "SMALLINT" };
	info.m_mapField["roadsize"] = { "roadsize", " SMALLINT UNSIGNED NOT NULL  COMMENT 'roadsize'", "SMALLINT" };
    info.m_mapField["hasflag"] = { "hasflag", " TINYINT UNSIGNED NOT NULL  COMMENT 'hasflag'", "TINYINT" };
    info.m_mapField["forkroad"] = { "forkroad", " SMALLINT UNSIGNED NOT NULL  COMMENT 'forkroad'", "SMALLINT" };
    info.m_mapField["classnum"] = { "classnum", " SMALLINT UNSIGNED NOT NULL  COMMENT 'classnum'", "SMALLINT" };
    info.m_mapField["information"] = { "information", " INT UNSIGNED NOT NULL  COMMENT 'information'", "INT" };
    info.m_mapField["name"] = { "name", "varchar(40) character set gbk collate gbk_chinese_ci NOT NULL COMMENT 'name'", "nvarchar(40)" };
    info.m_strKeyString = "PRIMARY KEY (LinkID), INDEX LinkID (LinkID)";
    m_vecTableInfo.push_back(info);//ֻ��һ����
}
bool CMysqlManager::init(const char* host, const char* user, const char* pwd, const char* dbname)
{
	m_strDataBase = dbname;
	if (!isDBExist())
	{
		if (!createDB())
		{
			return false;
		}
	}
	return true;
}

bool CMysqlManager::isDBExist()
{
	if (NULL == m_poConn)
	{
		return false;
	}
	QueryResult* pResult = m_poConn->query("show databases");
	if (NULL == pResult)
	{
		//LOGI << "CMysqlManager::_IsDBExist, no database(" << m_strDataBase << ")";
		return false;
	}
	Field* pRow = pResult->fetch();
	while (pRow != NULL)
	{
		std::string name = pRow[0].getString();
		if (name == m_strDataBase)
		{
			//LOGI << "CMysqlManager::_IsDBExist, find database(" << m_strDataBase << ")";
			pResult->endQuery();
			return true;
		}

		if (pResult->nextRow() == false)
		{
			break;
		}
		pRow = pResult->fetch();
	}
	//LOGI << "CMysqlManager::_IsDBExist, no database(" << m_strDataBase << ")";
	pResult->endQuery();
	delete pResult;
	return false;
}
bool CMysqlManager::createDB()
{
	if (NULL == m_poConn)
	{
		return false;
	}
	uint32_t uAffectedCount = 0;
	int nErrno = 0;
	std::stringstream ss;
	ss << "create database " << m_strDataBase;
	if (m_poConn->execute(ss.str().c_str(), uAffectedCount, nErrno))
	{
		if (uAffectedCount == 1)
		{
			//LOGI << "CMysqlManager::_CreateDB, create database " <<
			//	m_strDataBase << " success";
			return true;
		}
	}
	else
	{
		//LOGE << "CMysqlManager::_CreateDB, create database " << m_strDataBase << " failed("
		//	<< nErrno << ")";
		return false;
	}
	return false;
}