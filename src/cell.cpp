
bool TeFillCellCategoryCountPercentageRasterOperation(TeDatabase* db,
													  string raster_layername,
													  string cell_layername,
													  const string& cell_tablename,
													  const string attrName,
													  TeTimeInterval t)
{
	TeFillCellStepLoadingData();

	map<string, string> classesMap;
	vector< string > atts;
	string value;

	TeLayer* input_layer = new TeLayer (raster_layername);
	if (!db->loadLayer (input_layer))
	{
		 db->close();
		 return false;
	}

	atts.push_back(attrName);

	vector<double> values;
	bool found;
	unsigned int i;
	TeRaster* raster = input_layer->raster();
	TeRaster::iterator rasterItBegin = raster->begin();
	TeRaster::iterator rasterItEnd   = raster->end();

	TeTable table(cell_tablename);
	db->loadTableInfo(table);

	TeLayer* cell_layer = new TeLayer (cell_layername);
	if (!db->loadLayer (cell_layer))
	{
		 db->close();
		 return false;
	}

	// Initialize cell set
	TeCellSet cells;
	if (!TeFillCellInitLoad (cell_layer, cell_tablename, cells)) return false;

	// Initialize object set to store cell properties
	TeSTElementSet cellObjSet (cell_layer);

	// looking for the unique values in all the raster
	TeFillCellStepStartProcessing(raster->params().nlines_ * raster->params().ncols_ + cells.size());

	int count = 0;
	while(rasterItBegin != rasterItEnd)
	{
		found = false;
		for(i = 0; i < values.size(); ++i)
			if(values[i] == (*rasterItBegin)[0])
				found = true;
		if(!found) 
			values.push_back((*rasterItBegin)[0]);
		++rasterItBegin;

		if(!TeFillCellStepNextStep(++count)) return false;
	}

	// Process
	TePropertyVector result;
	map<double, int> counter;

	// set the propertyvector of the cell object set
	for(i = 0; i < values.size(); i++)
	{
		stringstream str;

		str << values[i];
        string value_ =  str.str();
        for (unsigned j = 0; j < value_.size(); j++)
            if(value_[j] == '.' || value_[j] == '+')
                value_[j] = '_';

        TeProperty prop;
        prop.attr_.rep_.name_ = attrName + value_;
        prop.attr_.rep_.type_ = TeREAL;
        prop.attr_.rep_.numChar_ = 48;
		prop.attr_.rep_.decimals_ = 10;

        cellObjSet.addProperty(prop.attr_);
	}

	TeCellSet::iterator cell_it = cells.begin();
	vector<string> vprop;

	while (cell_it != cells.end())
	{
		double total = 0.0;
		TePolygon p = polygonFromBox((*cell_it).box());
		TeRaster::iteratorPoly rasterItBegin(raster);
		rasterItBegin = raster->begin(p, TeBBoxPixelInters, 0);
		TeRaster::iteratorPoly rasterItEnd(raster);
		rasterItEnd = raster->end(p, TeBBoxPixelInters, 0);

		for(i = 0; i < values.size(); i++)
			counter[values[i]] = 0;

		while(rasterItBegin != rasterItEnd)
		{
			double pixel = (*rasterItBegin);
			counter[pixel]++;
			++rasterItBegin;
			total++;
		}		

		//add the attribute values in the object set
		vprop.clear();
		for(i = 0; i < values.size(); i++)
		{
			stringstream str;
			if (total > 0)
				str << counter[values[i]] / total;
			else
				str << "0.0";

			vprop.push_back(str.str());
		}

		TeFillCellInitSTO ((*cell_it), vprop, cellObjSet, table.tableType(), t);
		cell_it++;

		if(!TeFillCellStepNextStep(++count)) return false;
	} 

	TeFillCellStepWriting();
	// Update DB
	if (!TeUpdateDBFromSet (&cellObjSet, cell_tablename))
		return  false;
	return  true;
}



