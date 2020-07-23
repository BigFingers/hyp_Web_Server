#include "PhraseJson.h"
#include <sstream>
#include <fstream>

std::string ReturnHtml()
{
	std::string filename = "./list.txt";
	std::ifstream infile(filename);
	TemplateDictionary dict( "example");
	dict.SetValue ( "TABLE_NAME" , "信息表"  );
	if(!infile.is_open())
	{
		std::cout << "文件打开失败" << std::endl;
		return "";
	}

	std::string line;
	getline(infile, line);//去除第一行
	while(getline(infile, line))
	{
		std::string name;
		std::string sex;
		std::string email;
		std::istringstream str(line);
		str >> name >> sex >> email;

		TemplateDictionary *item = dict.AddSectionDictionary("TABLE_ITEM");
		item->SetValue ( "NAME" , TemplateString(name)  );
        item->SetValue ( "SEX" , TemplateString(sex)  );
        item->SetValue ( "EMAIL" , TemplateString(email)  );
	}
	dict.ShowSection ( "TABLE_NAME"  );
	Template * tpl = Template :: GetTemplate ( "example.tpl" , DO_NOT_STRIP );
	std::string output ;
	tpl->Expand(&output ,&dict);
	return output;
}

std::string ReturnJson()
{
	std::string filename = "./list.txt";
	std::ifstream infile(filename);
	if(!infile.is_open())
	{
		std::cout << "文件打开失败" << std::endl;
		return "";
	}

	const char* json = "{\"NAME\":\"test\",\"SEX\":\"test\",\"EMAIL\":\"test\"}";
	Document d;
	d.Parse(json);
	std::string result;

	std::string line;
	getline(infile, line);//去除第一行
	while(getline(infile, line))
	{
		std::string name_str;
		std::string sex_str;
		std::string email_str;
		std::istringstream str(line);
		str >> name_str >> sex_str >> email_str;

		Value& name = d["NAME"];
		name.SetString(name_str.c_str(),name_str.length());
		Value& sex = d["SEX"];
		sex.SetString(sex_str.c_str(),sex_str.length());
		Value& email = d["EMAIL"];
		email.SetString(email_str.c_str(),email_str.length());
		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		d.Accept(writer);
		result += buffer.GetString();
		result += "\n";
	}
	return result;
	
}

std::string PhraseJson(const std::string& form)
{
	if(form == "html")
		return ReturnHtml();
	if(form == "json")
		return ReturnJson();
}

