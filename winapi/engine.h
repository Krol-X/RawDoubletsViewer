//
// ��������� ������: ENGINE.CPP
//
// ����������: �������� ��� ������ � PhoneDB
//

#ifndef engine_h_
#define engine_h_

#include "include.h"
#include <vector>

struct LPFileHeader {
	INT64 AllocatedLinks, ReservedLinks,
	      FreeLinks, FirstFreeLink,
	      RootAsSource, RootAsTarget,
	      LastFreeLink, Reserved;
};

struct LPItem {
	INT64 Source, Target,
	      LeftAsSource, RightAsSource, SizeAsSource,
	      LeftAsTarget, RightAsTarget, SizeAsTarget;
};

class LinksPlatformDB {
	public:
		LPFileHeader header;
		std::vector<LPItem> item;
		
	public:
		LinksPlatformDB();
		~LinksPlatformDB();
		
		UINT length();                // ���������� ������� � ��
		void clear();                 // �������� ��

		bool load(LPTSTR);            // ��������� ������ �� �����
		void load(LinksPlatformDB&);  // ��������� ������ �� ������ ��
};


#endif
