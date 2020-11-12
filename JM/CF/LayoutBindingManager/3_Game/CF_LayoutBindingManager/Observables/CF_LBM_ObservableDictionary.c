class CF_LBM_ObservableDictionary<Class TKey, Class TValue> : CF_LBM_Observable
{
	private	ref map<TKey, TValue> _data = new map<TKey, TValue>();

	bool Insert(TKey key, TValue value)
	{
		if (_data.Insert(key, value))
		{
			CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Add, new Param2<TKey, TValue>(key, value)));
			return true;
		}

		return false;
	}

	void Remove(TKey key)
	{
		if (_data.Contains(key))
		{
			TValue value = _data.Get(key);
			_data.Remove(key);
			CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Remove, new Param2<TKey, TValue>(key, value)));
		}
	}

	void Clear()
	{
		_data.Clear();
		CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Clear, null));
	}

	void Set(TKey key, TValue value)
	{
		_data.Set(key, value);
		CollectionChanged(new CF_LBM_CollectionChangedEventArgs(this, CF_LBM_NotifyCollectionChangedAction.Set, new Param2<TKey, TValue>(key, value)));
	}

	TValue Get(TKey key)
	{
		return _data.Get(key);
	}

	TKey GetKey(int index)
	{
		return _data.GetKey(index);
	}

	override int Count()
	{
		return _data.Count();
	}

	override typename GetType()
	{
		return TValue;
	}
};