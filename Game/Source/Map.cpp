#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"


#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false) { name.Create("map"); }

// Destructor
Map::~Map() {}

void Map::Init() { active = false; }

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;
	folder.Create(config.child("folder").child_value());
	return ret;
}

// Draw the map (all requried layers)

void Map::Draw()
{
	if (mapLoaded == false) return;

	ListItem <MapLayer*>* layer;
	layer = data.layers.start;
	TileSet* currentTileset;

	while (layer != NULL)
	{
		
		if (layer->data->properties.GetProperty("Drawable") == 1 || app->render->drawLayerColliders)
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					uint tileId = layer->data->Get(x, y);
					if (tileId > 0)
					{
						// L04: TODO 9: Complete the draw function
						currentTileset = GetTilesetFromTileId(tileId);
						SDL_Rect tileRec = currentTileset->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						app->render->DrawTexture(currentTileset->texture, pos.x, pos.y, &tileRec);
					}
				}
			}
		}
		layer = layer->next;
	}
}



// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tileWidth;
	ret.y = y * data.tileHeight;
	return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };
	// L04:  Get relative Tile rectangle
	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));
	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
	// Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.Clear();


	// L04: DONE 2: clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.Clear();

	// Clean up the pugui tree
	mapFile.reset();

	return true;
}

// Load new map
bool Map::Load(const char* filename)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	if(ret == true)
	{
		ret = LoadMap();
		pugi::xml_node tileset;
		for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
		{
			TileSet* set = new TileSet();
			if (ret == true) ret = LoadTilesetDetails(tileset, set);
			if (ret == true) ret = LoadTilesetImage(tileset, set);
			ret = LoadTilesetProperties(tileset, set);
			data.tilesets.Add(set);
		}

		//Iterate all layers and load each of them

		for (pugi::xml_node layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
		{
			MapLayer* lay = new MapLayer();
							
			ret = LoadLayer(layer, lay);
			if (ret == true) data.layers.Add(lay);
		}
		LogInfo();
	}

	mapLoaded = ret;

	return ret;
}

//Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");
	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.height = map.attribute("height").as_int();
		data.width = map.attribute("width").as_int();
		SString type(map.attribute("orientation").as_string());
		data.type = StrToMapType(type);
		data.tileHeight = map.attribute("tileheight").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
	}
	return ret;
}

//Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tilesetNode, TileSet* set)
{
	bool ret = true;
	LOG("filling TilesetDetails");

	set->name.Create(tilesetNode.attribute("name").as_string());
	set->firstgid = tilesetNode.attribute("firstgid").as_int();
	set->tileWidth = tilesetNode.attribute("tilewidth").as_int();
	set->tileHeight = tilesetNode.attribute("tileheight").as_int();
	set->spacing = tilesetNode.attribute("spacing").as_int();
	set->margin = tilesetNode.attribute("margin").as_int();

	return ret;
}

//Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tilesetNode, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tilesetNode.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{

		//SString path = folder.GetString();
		//path += image.attribute("source").as_string();

		SString path("%s%s", folder.GetString(), image.attribute("source").as_string());

		set->texture = app->tex->Load(path.GetString());
		set->texWidth = image.attribute("width").as_int();
		set->texHeight = image.attribute("height").as_int();

		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
		set->offsetX = 0;
		set->offsetY = 0;
	}
	return ret;
}

bool Map::LoadTilesetProperties(pugi::xml_node& node, TileSet* set)
{
	bool ret = true;
	for (pugi::xml_node tileNode = node.child("tile"); tileNode && ret; tileNode = tileNode.next_sibling("tile"))
	{
		Tile* tileProperties = new Tile;
		tileProperties->id = tileNode.attribute("id").as_int();

		ret = LoadProperties(tileNode.child("properties"), tileProperties->properties);
		set->tilesetPropList.Add(tileProperties);
	}
	return ret;
}

bool Map::StoreId(pugi::xml_node& node, MapLayer* layer, int index)
{
	bool ret = true;

	layer->data[index] = node.attribute("gid").as_uint(0);

	return ret;
}

//Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	//Load a single layer
	layer->name.Create(node.attribute("name").as_string());
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();

	pugi::xml_node layerData = node.child("data");

	if (layerData == NULL)
	{
		LOG("Error loading node child data, inside LoadLayer");
		ret = false;
	}
	else
	{
		layer->data = new uint[layer->width * layer->height];
		memset(layer->data, 0, layer->width * layer->height * sizeof(uint));
		pugi::xml_node gidNode;

		int i = 0;
		for (gidNode = node.child("data").child("tile"); gidNode && ret; gidNode = gidNode.next_sibling("tile"))
		{
			if (ret == true) ret = StoreId(gidNode, layer, i);
			++i;
		}
		LOG("Layer <<%s>> has loaded %d tiles", layer->name.GetString(), i);
	}
	ret = LoadProperties(node.child("properties"), layer->properties);
	return ret;
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;

	pugi::xml_node property;
	for (property = node.child("property"); property; property = property.next_sibling("property"))
	{
		Properties::Property* prop = new Properties::Property();

		prop->name = property.attribute("name").as_string();
		prop->value = property.attribute("value").as_int();
		properties.list.Add(prop);
	}
	return ret;
}

MapTypes operator++(MapTypes& mode)
{
	mode = static_cast<MapTypes>((mode + 1) % 4);
	return mode;
}

MapTypes Map::StrToMapType(SString s)
{
	SString StrType[4];
	StrType[0] = "unknown", StrType[1] = "orthogonal", StrType[2] = "isometric", StrType[3] = "staggered";
	MapTypes type = MapTypes::MAPTYPE_UNKNOWN;
	for (int i = 0; i < 4; ++i)
	{
		if (s == StrType[i])
		{
			return ++type;
		}
	}
	return MAPTYPE_UNKNOWN;
}

TileSet* Map::GetTilesetFromTileId(int id) const 
{
	ListItem<TileSet*>* item = data.tilesets.end;
	TileSet* set = item->data;

	while (item != NULL) 
	{
		if (set->firstgid <= id) { return set; }
		item = item->prev;
		set = item->data;
	}
	return set;
}

Tile* TileSet::GetPropList(int id) const {
	ListItem<Tile*>* tile = tilesetPropList.start;
	Tile* t = nullptr;
	while (tile != NULL) {
		t = tile->data;
		if (t->id == id) {
			return t;
		}
		tile = tile->next;
	}
	return t;
}

int Properties::GetProperty(const char* value, int default) const
{
	
	ListItem<Property*>* PropertiesL;
	PropertiesL = list.start;

	SString proppr = value;

	while (PropertiesL != NULL)
	{
		//LOG("Checking property: %s", P->data->name.GetString());         //<- checks the property
		if (PropertiesL->data->name == proppr)
		{
			return PropertiesL->data->value;
		}
		PropertiesL = PropertiesL->next;
	}
	return default;
}

void Map::LogInfo()
{
	// LOG all the data loaded
	LOG("--------------------------------------------------------------------------");
	LOG("<< MAP DATA >>");
	LOG("Width=%d", data.width);
	LOG("Height=%d", data.height);
	LOG("TileWidth=%d", data.tileWidth);
	LOG("TileHeight=%d", data.tileHeight);
	LOG("<< END MAP DATA >>\n");

	ListItem<TileSet*> * infoList;
	infoList = data.tilesets.start;
	while (infoList != NULL)
	{
		LOG("<< TILESET >>");
		LOG("Name=%s", infoList->data->name.GetString());
		LOG("Firstgid=%d", infoList->data->firstgid);
		LOG("Margin=%d", infoList->data->margin);
		LOG("Spacing=%d", infoList->data->spacing);
		LOG("Tile_width=%d", infoList->data->tileWidth);
		LOG("Tile_height=%d", infoList->data->tileHeight);
		LOG("texWidth=%d", infoList->data->texWidth);
		LOG("texHeight=%d", infoList->data->texHeight);
		LOG("numTilesWidth=%d", infoList->data->numTilesWidth);
		LOG("numTilesHeight=%d", infoList->data->numTilesHeight);
		LOG("<< END TILESET >>\n");

		infoList = infoList->next;
	}

	// LOG the info for each loaded layer
	ListItem<MapLayer*>*layerList;
	layerList = data.layers.start;
	while (layerList != NULL)
	{
		LOG("<< LAYER >>");
		LOG("Name=%s", layerList->data->name.GetString());
		LOG("Width=%d", layerList->data->width);
		LOG("Height=%d", layerList->data->height);
		LOG("<< END LAYER >>\n");
		layerList = layerList->next;
	}
	LOG("--------------------------------------------------------------------------");
}


void Properties::SetProperty(const char* name, int value)
{
	ListItem<Property*>* property;
	property = list.start;

	SString prop = name;

	while (property != NULL)
	{
		//LOG("Checking property: %s", P->data->name.GetString());
		if (property->data->name == prop)
		{
			property->data->value = value;
			return;
		}
		property = property->next;
	}
}


void Map::SetTileProperty(int x, int y, const char* property, int value, bool nonMovementCollision, bool isObject)
{
	// MapLayer
	ListItem <MapLayer*>* mapLayer = data.layers.start;
	SString layerName;
	if (isObject)
	{
		layerName = "Level_1";
	}
	else
	{
		layerName = "Collisions";
	}

	while (mapLayer != NULL)
	{
		if (mapLayer->data->name == layerName)
		{
			break;
		}
		mapLayer = mapLayer->next;
	}

	// TileSet
	ListItem <TileSet*>* tileSet = data.tilesets.start;
	SString tileSetName;
	if (nonMovementCollision)
	{
		tileSetName = "LevelTileset";
	}
	else
	{
		tileSetName = "Metadata";
	}
	
	while (tileSet != NULL)
	{
		if (tileSet->data->name == tileSetName)
		{
			break;
		}
		tileSet = tileSet->next;
	}

	// Gets Collider
	int id = (int)(mapLayer->data->Get(x, y) - tileSet->data->firstgid);
	if (id < 0)
	{
		return;
	}
	Tile* currentTile = tileSet->data->GetPropList(id);
	currentTile->properties.SetProperty(property, value);
}



int Map::GetTileProperty(int x, int y, const char* property, bool nonMovementCollision, bool isObject) const
{
	int ret;
	// MapLayer
	ListItem <MapLayer*>* mapLayer = data.layers.start;
	SString layerName;
	if (isObject)
	{
		layerName = "Level_1";
	}
	else
	{
		layerName = "Collisions";
	}

	while (mapLayer != NULL)
	{
		if (mapLayer->data->name == layerName)
		{
			break;
		}
		mapLayer = mapLayer->next;
	}

	// TileSet
	ListItem <TileSet*>* tileSet = data.tilesets.start;
	SString tileSetName;
	if (nonMovementCollision)
	{
		tileSetName = "LevelTileset";
	}
	else
	{
		tileSetName = "Metadata";
	}

	while (tileSet != NULL)
	{
		if (tileSet->data->name == tileSetName)
		{
			break;
		}
		tileSet = tileSet->next;
	}

	// Gets CollisionId
	int id = (int)(mapLayer->data->Get(x, y) - tileSet->data->firstgid);
	if (id < 0)
	{
		ret = 0;
		return ret;
	}
	Tile* currentTile = tileSet->data->GetPropList(id);
	ret = currentTile->properties.GetProperty(property, 0);
	return ret;
}