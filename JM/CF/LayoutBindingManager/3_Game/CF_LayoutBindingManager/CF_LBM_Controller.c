/*

Controller Class Example:

class TestController: CF_LBM_Controller
{
	// "TextBox1" goes into Binding_Name
	string TextBox1 = "TextBox1Text";
	
	// "TextBox2" goes into Binding_Name
	int TextBox2;
	
	// "WindowButton1" goes into Binding_Name
	bool WindowButton1; 
	
	
	// If WindowButton1 is bound to a button,
	// call this function and it will
	// set the state of the button on your UI
	void SetWindowButton1(bool state)
	{
		WindowButton1 = state;
		NotifyPropertyChanged("WindowButton1");
	}
	
	// Gets called whenever a property was changed in the controller
	override void PropertyChanged(string propertyName)
	{
		switch (propertyName)
		{
			case "WindowButton1": {
				Print("WindowButton1 " + WindowButton1);
				break;
			}
		}
	}
}
*/

// Abstract Class
class CF_LBM_Controller : CF_LBM_ScriptedViewBase
{
	// All View Bindings
	[NonSerialized()]
	protected autoptr CF_LBM_ViewBindingHashMap m_ViewBindingHashMap = new CF_LBM_ViewBindingHashMap();
	CF_LBM_ViewBindingHashMap GetViewBindings()
	{
		return m_ViewBindingHashMap;
	}

	CF_LBM_ViewBinding GetViewBinding(Widget source)
	{
		return m_ViewBindingHashMap.Get(source);
	}

	// View Bindings indexed by their Binding_Name
	[NonSerialized()]
	protected autoptr CF_LBM_DataBindingHashMap m_DataBindingHashMap = new CF_LBM_DataBindingHashMap();
	CF_LBM_DataBindingHashMap GetDataBindings()
	{
		return m_DataBindingHashMap;
	}

	// Hashmap of all properties in the Controller
	[NonSerialized()]
	protected autoptr CF_LBM_PropertyTypeHashMap m_PropertyTypeHashMap = CF_LBM_PropertyTypeHashMap.FromType(Type());
	typename GetPropertyType(string propertyName)
	{
		return m_PropertyTypeHashMap.Get(propertyName);
	}

	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);

		//m_PropertyTypeHashMap.RemoveType(CF_LBM_Controller); crashing?

		// Load all child Widgets and obtain their DataBinding class
		int binding_count = LoadDataBindings(m_LayoutRoot);
		Log("%1: %2 DataBindings found!", m_LayoutRoot.GetName(), binding_count.ToString());
	}

	/*
	* Example:
	*	NotifyPropertyChanged("slider_value");
	*
	* Call this when you update a Controller property (variable)
	* Do NOT call this when using arrays / collections. Use CF_LBM_ObservableCollection!	
	*
	* Calling this with No Parameters will update ALL properties in the Controller. 
	* This is NOT recommended because it is VERY resource intensive
	*
	*/

	void NotifyPropertyChanged(string property_name = "", bool notify_controller = true)
	{
		// Did you know that when the compiler checks for ambiguous types, it uses string.Contains()
		// instead of string.Match()? PropertyChanged and NotifyPropertyChanged need to be distinct or
		// the whole damn thing breaks. Thanks BI
		Trace("NotifyPropertyChanged %1", property_name);

		if (property_name == string.Empty)
		{
			Log("Updating all properties in View, this is NOT recommended as it is performance intensive");
			foreach (CF_LBM_ViewBindingSet viewArray : m_DataBindingHashMap)
			{
				foreach (CF_LBM_ViewBinding viewBinding : viewArray)
				{
					Trace("NotifyPropertyChanged %1", viewBinding.Binding_Name);
					viewBinding.UpdateView(this);
					PropertyChanged(viewBinding.Binding_Name);
				}
			}

			return;
		}

		CF_LBM_ViewBindingSet views = m_DataBindingHashMap[property_name];
		if (views)
		{
			foreach (CF_LBM_ViewBinding view : views)
			{
				view.UpdateView(this);
			}
		}

		if (notify_controller)
		{
			PropertyChanged(property_name);
		}
	}

	// Do NOT call this. CF_LBM_ObservableCollection does this for you
	void NotifyCollectionChanged(CF_LBM_CollectionChangedEventArgs args)
	{
		Trace("NotifyCollectionChanged %1", args.Source.ToString());

		string collection_name = GetVariableName(args.Source);
		if (collection_name == string.Empty)
		{
			Error("NotifyCollectionChanged could not find variable %1 in %2", args.Source.ToString(), string.ToString(this));
			return;
		}

		CF_LBM_ViewBindingSet views = m_DataBindingHashMap[collection_name];

		if (views)
		{
			foreach (CF_LBM_ViewBinding view : views)
			{
				view.UpdateViewFromCollection(args);
			}
		}

		CollectionChanged(collection_name, args);
	}

	// Gets called every time a property is changed.
	// Override this when you want to have an event AFTER property is changed
	void PropertyChanged(string property_name);

	// Gets called every time an observable collection is changed.
	// Override this when you want to have an event AFTER collection is changed
	void CollectionChanged(string collection_name, CF_LBM_CollectionChangedEventArgs args);

	private int LoadDataBindings(Widget w)
	{
		CF_LBM_ScriptedViewBase viewBase;
		w.GetScript(viewBase);

		// If we find a CF_LBM_ViewBinding
		if (viewBase && viewBase.IsInherited(CF_LBM_ViewBinding))
		{
			CF_LBM_ViewBinding viewBinding = CF_LBM_ViewBinding.Cast(viewBase);
			viewBinding.SetParent(this);
			m_ViewBindingHashMap.Insert(w, viewBinding);
			m_DataBindingHashMap.InsertView(viewBinding);

			viewBinding.SetProperties(GetControllerProperty(viewBinding.Binding_Name), GetControllerProperty(viewBinding.Selected_Item));

			// todo find a way to define these on CF_LBM_ScriptView aswell
			// Load CF_LBM_RelayCommand
			if (viewBinding.Relay_Command != string.Empty)
			{

				CF_LBM_RelayCommand relayCommand = LoadRelayCommand(viewBinding);
				// Success! One of the two options were found
				if (relayCommand)
				{
					Log("%2: RelayCommand %1 succesfully acquired. Assigning...", viewBinding.Relay_Command, viewBinding.GetLayoutRoot().GetName());
					relayCommand.SetController(this);
					viewBinding.SetRelayCommand(relayCommand);
				} else // Must be a function on the controller
				{
					Log("%2: RelayCommand %1 not found - Assuming its a function on the Controller / CF_LBM_ScriptView!", viewBinding.Relay_Command, viewBinding.GetLayoutRoot().GetName());
				}
			}

			// Load property for the first time
			if (viewBinding.Binding_Name != string.Empty)
			{
				NotifyPropertyChanged(viewBinding.Binding_Name, false);
			}
		}

		// really wish i had XOR here
		bool b1 = (w.GetChildren() != null);
		bool b2 = (viewBase && viewBase.IsInherited(CF_LBM_Controller) && viewBase != this);

		// scuffed XOR
		// Makes it stop loading when it finds another controller
		// needs to be looked at
		if (b1 && (b1 || b2) && !(b1 && b2))
		{
			LoadDataBindings(w.GetChildren());
		} else if (b2) // Sets parent of the child controller
		{
			CF_LBM_Controller childController;
			if (Class.CastTo(childController, viewBase))
			{
				childController.SetParent(this);
			}
		}

		// w != m_LayoutRoot is so we dont bleed into siblings
		if (w.GetSibling() != null && w != m_LayoutRoot)
		{
			LoadDataBindings(w.GetSibling());
		}

		return m_DataBindingHashMap.Count();
	}

	private	typename GetControllerProperty(string propertyName)
	{
		if (m_PropertyTypeHashMap[propertyName])
		{
			return m_PropertyTypeHashMap[propertyName];
		}

		// Searches properties for Sub properties
		Class context = this;
		return GetControllerProperty(context, propertyName);
	}

	private	typename GetControllerProperty(out Class context, string propertyName)
	{
		CF_LBM_PropertyInfo propertyInfo = CF_LBM.GetSubScope(context, propertyName);
		if (propertyInfo)
		{
			return propertyInfo.Type;
		}

		typename t;
		return t;
	}

	private	string GetVariableName(Class targetVariable)
	{
		typename type = Type();
		for (int i = 0; i < type.GetVariableCount(); i++)
		{
			typename variableType = type.GetVariableType(i);
			string variableName = type.GetVariableName(i);

			if (!variableType.IsInherited(targetVariable.Type()))
				continue;

			Class result;
			EnScript.GetClassVar(this, variableName, 0, result);
			if (result == targetVariable)
			{
				return variableName;
			}
		}

		return string.Empty;
	}

	private	CF_LBM_RelayCommand LoadRelayCommand(CF_LBM_ViewBinding viewBinding)
	{
		string relayCommandName = viewBinding.Relay_Command;
		CF_LBM_RelayCommand relayCommand;

		// Attempt to load instance of Variable from Controller
		Class context = this;
		CF_LBM_PropertyInfo relayCommandProperty = CF_LBM.GetSubScope(context, relayCommandName);
		if (relayCommandProperty)
		{
			typename relayCommandType = relayCommandProperty.Type;
			relayCommandName = relayCommandProperty.Name;

			// If we find the variable on the Controller
			if (relayCommandType && relayCommandType.IsInherited(CF_LBM_RelayCommand))
			{
				Log("RelayCommand Property %1 found on Controller!", relayCommandName);
				EnScript.GetClassVar(context, relayCommandName, 0, relayCommand);

				// If that property isnt initialized, but exists
				if (!relayCommand)
				{
					Log("RelayCommand Property %1 was not initialized! Initializing...", relayCommandName);
					Class.CastTo(relayCommand, relayCommandType.Spawn());
					EnScript.SetClassVar(context, relayCommandName, 0, relayCommand);
					return relayCommand;
				}
			}
		} else // If we DONT find the variable on the controller, attempt to create an instance of it
		{
			Log("RelayCommand Property %1 not found on Controller", relayCommandName);
			relayCommandType = relayCommandName.ToType();

			if (relayCommandType && relayCommandType.IsInherited(CF_LBM_RelayCommand))
			{
				Log("RelayCommand type found %1", relayCommandName);
				Class.CastTo(relayCommand, relayCommandType.Spawn());
				return relayCommand;
			}
		}

		return relayCommand;
	}

	// Update Controller on action from CF_LBM_ViewBinding
	override bool OnClick(Widget w, int x, int y, int button)
	{
		CF_LBM_ViewBinding viewBinding = m_ViewBindingHashMap.Get(w);
		if (viewBinding)
		{
			viewBinding.UpdateController(this);
		}

		return super.OnClick(w, x, y, button);
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		CF_LBM_ViewBinding viewBinding = m_ViewBindingHashMap.Get(w);
		if (viewBinding)
		{
			viewBinding.UpdateController(this);
		}

		return super.OnChange(w, x, y, finished);
	}

	// Two way binding interfaces
	// Specifically for SpacerBaseWidget
	/*
	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		CF_LBM_ViewBinding reciever_view;
		reciever.GetScript(reciever_view);
		//CF_LBM_ScriptedViewBase.FindScriptedRoot(reciever_view);
		if (reciever_view) {
			
			CF_LBM_ScriptedViewBase scripted_view;
			w.GetScript(scripted_view);
			CF_LBM_ScriptedViewBase.FindScriptedRoot(scripted_view);
			if (scripted_view) {
				reciever_view.HandleDropReceived(scripted_view, this);
			}
		}
		
		return super.OnDropReceived(w, x, y, reciever);
	}
	*/

	void DebugPrint()
	{
		m_DataBindingHashMap.DebugPrint();
	}
};