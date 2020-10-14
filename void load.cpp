    /*******************************************/
  /**** XML Student Behaviour structures *************/
  /*******************************************/
  class DecisionMakerBehaviour{
  public:
    DecisionMakerBehaviour() = default;
    string                         bannerId;
    string                         location;
    string                         reactionTime;
  	string						   size;
	string						   personId;
	string						   relationshipType;
	string						   personStatus;
	string						   safeDistanceProbability;
	string						   maskProbability;
	string						   group;
	
   // vector<ProbabilityOfSafeDistance>   safeDistanceProbability;
	//vector<ProbabilityOfWearingMask>	maskProbability;
	//vector<UniversityStatus>			personStatus;
	//vector<string>						relationshipType;
	//vector<NumberOfPeople>				group;
	//vector<RoomSize>					size;
	

  
   void load(const char* pFilename){     
      //cout << "entra aqui load" << endl;         
      TiXmlDocument _document;
      bool result = _document.LoadFile(pFilename);
      if (!result) {cout << "file not loaded" << _document.ErrorDesc() << endl; return;}   
      TiXmlHandle hDoc(&_document);
      TiXmlElement* pElem;
      TiXmlHandle hRoot(0);
      // block: root
        {
          pElem=hDoc.FirstChildElement().Element();
          // should always have a valid root but handle gracefully if it does
          if (!pElem) return;    
          // save this for later
          hRoot=TiXmlHandle(pElem);
        }      
      // block: Id
        { 
          pElem=hRoot.FirstChild("Id").Element();
          if (!pElem) return;
          const char* pId = pElem->GetText();
          if(pId) id = pId;
        }
		// block: BannerID
        { 
          pElem=hRoot.FirstChild("BannerID").Element();
          if (!pElem) return;
          const char* pBannerID = pElem->GetText();
          if(pBannerID) bannerid = pBannerID;
        }
       // block: Location
        { 
          pElem=hRoot.FirstChild("Location").Element();
          if (!pElem) return;
          const char* pLocation = pElem->GetText();
          if(pLocation) location = pLocation;
        }
      // block: ReactionTime
        {
          pElem=hRoot.FirstChild("ReactionTime").Element();
          if (!pElem) return;
          const char* pReactionTime = pElem->GetText();
          if(pReactionTime) reactionTime = pReactionTime;
        }
		// block: Size    
        { 
          pElem=hRoot.FirstChild("Size").Element();
          if (!pElem) return;
          const char* pLocation = pElem->GetText();
          if(pSize) size = pSize;
        }
		// block: PersonID
        { 
          pElem=hRoot.FirstChild("PersonID").Element();
          if (!pElem) return;
          const char* pPersonID = pElem->GetText();
          if(pPersonID) personid = pPersonID;
        }
		// block: RelationshipType
        { 
          pElem=hRoot.FirstChild("RelationshipType").Element();
          if (!pElem) return;
          const char* pRelationshipType = pElem->GetText();
          if(pRelationshipType) relationshipType = pRelationshipType;
        }
		// block: PersonStatus
        { 
          pElem=hRoot.FirstChild("PersonStatus").Element();
          if (!pElem) return;
          const char* pPersonStatus = pElem->GetText();
          if(pPersonStatus) personStatus = pPersonStatus;
        }
		// block: SafeDistanceProbability
        { 
          pElem=hRoot.FirstChild("SafeDistanceProbability").Element();
          if (!pElem) return;
          const char* pSafeDistanceProbability = pElem->GetText();
          if(pSafeDistanceProbability) safeDistanceProbability = pSafeDistanceProbability;
        }
		// block: MaskProbability
        { 
          pElem=hRoot.FirstChild("MaskProbability").Element();
          if (!pElem) return;
          const char* pMaskProbability = pElem->GetText();
          if(pMaskProbability) maskProbability = pMaskProbability;
        }
		// block: Group
        { 
          pElem=hRoot.FirstChild("Group").Element();
          if (!pElem) return;
          const char* pGroup = pElem->GetText();
          if(pGroup) group = pGroup;
        }
	}
		
		
		