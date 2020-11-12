
enum CF_LBM_NotifyCollectionChangedAction
{
	Insert,
	InsertAt,
	Remove,
	Replace,
	Move,
	Swap,
	Clear
};

// 0: Start Index
// 1: Finish Index
typedef Param2<int, int> CF_LBM_CollectionSwapArgs;

// Event Args for Collection Changed
// 0: Collection that was changed
// 1: Collection Changed Action
// 2: Collection Changed Index
// 3: Collection Changed Value
class CF_LBM_CollectionChangedEventArgs
{
	CF_LBM_Observable Source;
	CF_LBM_NotifyCollectionChangedAction ChangedAction;	
	int ChangedIndex;
	Param ChangedValue;
	
	void CF_LBM_CollectionChangedEventArgs(CF_LBM_Observable source, CF_LBM_NotifyCollectionChangedAction changedAction, int changedIndex, Param changedValue)
	{
		Source = source;
		ChangedAction = changedAction;
		ChangedIndex = changedIndex;
		ChangedValue = changedValue;
	}
}