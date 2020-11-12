
class CF_LBM_PropertyInfo
{
	string Name;
	typename Type;
	
	void CF_LBM_PropertyInfo(string name, typename type)
	{
		Name = name;
		Type = type;
	}
	
	static CF_LBM_PropertyInfo GetFromClass(Class context, string name)
	{
		if (!context)
			return null;

		CF_LBM_PropertyTypeHashMap hash_map = CF_LBM_PropertyTypeHashMap.FromType(context.Type());
		if (hash_map[name])
		{
			return new CF_LBM_PropertyInfo(name, hash_map[name]);
		}
		
		return null;
	}
	
	static CF_LBM_PropertyInfo GetFromType(typename parent_type, string name)
	{
		CF_LBM_PropertyTypeHashMap hash_map = CF_LBM_PropertyTypeHashMap.FromType(parent_type);
		if (hash_map[name])
		{
			return new CF_LBM_PropertyInfo(name, hash_map[name]);
		}
		
		return null;
	}
}


// 0: Property Name
// 1: Property Type
class CF_LBM_PropertyTypeHashMap: map<string, typename>
{
	static CF_LBM_PropertyTypeHashMap FromType(typename type)
	{
		CF_LBM_PropertyTypeHashMap hash_map = new CF_LBM_PropertyTypeHashMap();
		for (int i = 0; i < type.GetVariableCount(); i++)
		{
			hash_map.Insert(type.GetVariableName(i), type.GetVariableType(i));	
		}
		
		return hash_map;
	}
	
	void RemoveType(typename removed_type)
	{
		CF_LBM_PropertyTypeHashMap hash_map = FromType(removed_type);
		foreach (string name, typename type: hash_map)
			Remove(name);
	}
}


// 0: Source Widget
// 1: View Binding
typedef map<Widget, CF_LBM_ViewBinding> CF_LBM_ViewBindingHashMap;

// 0: View Binding
typedef set<CF_LBM_ViewBinding> CF_LBM_ViewBindingSet;

// 0: Property Name
// 1: View Binding Set
class CF_LBM_DataBindingHashMap: map<string, autoptr CF_LBM_ViewBindingSet>
{
	void DebugPrint()
	{
		foreach (string name, CF_LBM_ViewBindingSet viewSet: this)
		{
			CF_LBM.Log("[%1]:", name);
			foreach (CF_LBM_ViewBinding view: viewSet)
			{
				CF_LBM.Log("    %1", view.GetLayoutRoot().GetName());
			}
		}
	}
	
	void InsertView(CF_LBM_ViewBinding view)
	{
		CF_LBM_ViewBindingSet viewSet = Get(view.Binding_Name);
		if (!viewSet)
		{
			viewSet = new CF_LBM_ViewBindingSet();
			viewSet.Insert(view);
			Insert(view.Binding_Name, viewSet);
			Insert(view.Selected_Item, viewSet);
		} else
		{
			viewSet.Insert(view);
		}		
	}
}

// 0: Relay_Command parameter
// 1: Command Value
typedef map<string, CF_LBM_RelayCommand> CF_LBM_RelayCommandHashMap

// 0: Source Type
// 1: Conversion Type
typedef map<typename, typename> CF_LBM_TypenameHashMap;

// 0: Source Type
// 1: Conversion Type
class CF_LBM_TypeConversionHashMap
{
	private autoptr map<typename, typename> value = new map<typename, typename>();
	
	typename Get(typename conversionType)
	{
		typename result = value.Get(conversionType);
		
		if (!result)
		{
			foreach (typename type, typename conversion: value)
			{
				if (conversionType.IsInherited(type))
				{
					return conversion;
				}
			}
		}
		
		return result;
	}
	
	void Remove(typename conversionType) {
		value.Remove(conversionType);
	}
	
	void Set(typename conversionType, typename conversionClass)
	{
		if (!conversionClass.IsInherited(CF_LBM_TypeConversionTemplate))
		{
			CF_LBM.Error(string.Format("TypeConverterHashMap: %1 must inherit from type CF_LBM_TypeConversionTemplate", conversionClass.ToString()));
			return;
		}
		
		value.Set(conversionType, conversionClass);
	} 
	
	bool Insert(typename conversionType, typename conversionClass)
	{
		if (!conversionClass.IsInherited(CF_LBM_TypeConversionTemplate))
		{
			CF_LBM.Error(string.Format("TypeConverterHashMap: %1 must inherit from type CF_LBM_TypeConversionTemplate", conversionClass.ToString()));
			return false;
		}
		
		return value.Insert(conversionType, conversionClass);
	}
};