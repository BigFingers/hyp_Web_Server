#include "PhraseJson.h"



std::string ReturnHtml()
{
	TemplateDictionary dict( "example");
	dict.SetValue ( "TABLE_NAME" , "信息表"  );
	for(int i=0; i<10; i++)
	{
		TemplateDictionary *item = dict.AddSectionDictionary("TABLE_ITEM");
		item->SetValue ( "NAME" , "1111111"  );
                item->SetValue ( "SEX" , "1111111"  );
                item->SetValue ( "EMAIL" , "1111111"  );
	}
	dict.ShowSection ( "TABLE_NAME"  );
	Template * tpl = Template :: GetTemplate ( "example.tpl" , DO_NOT_STRIP );
	std::string output ;
	tpl->Expand(&output ,&dict);
	return output;
}

std::string ReturnJson()
{
	const char* json = "{\"NAME\":\"test\",\"SEX\":\"test\",\"EMAIL\":\"test\"}";
	Document d;
	d.Parse(json);

	Value& name = d["NAME"];
	name.SetString("zhangsan");
	Value& sex = d["SEX"];
	sex.SetString("man");
	Value& email = d["EMAIL"];
	email.SetString("zhangsan@baidu.com");

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);
	std::string result = buffer.GetString();
	return result;
	
}

std::string PhraseJson(const std::string& form)
{
	if(form == "html")
		return ReturnHtml();
	if(form == "json")
		return ReturnJson();
}

