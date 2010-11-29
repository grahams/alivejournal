class BList;

class ResponseBlockManager
{
public:
						ResponseBlockManager(char *http_response);
						~ResponseBlockManager(void);
	const char *		GetValue(const char *name, bool decode_url=false);
	
private:
	BList *				hash_values;
};
