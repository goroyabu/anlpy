{
    auto tree = (TTree*)gDirectory->Get("tree");
    cout << "tree entries = " << tree->GetEntries() << endl;
    
    auto list = tree->GetListOfLeaves();

    auto n = list->GetEntries();
    cout << "n=" << n << endl;
    
    for ( int i=0; i<n; ++i ) {
	auto obj = list->At(i);
	if ( !obj->InheritsFrom("TLeaf") ) continue;
	
	auto leaf = (TLeaf*)obj;
	auto count_name = "";
	if ( leaf->GetLeafCount() ) count_name = leaf->GetLeafCount()->GetName();
	//auto val = leaf->GetTypedValue();
	
	cout << setw(30) << leaf->GetName() << " " << setw(40) << leaf->GetTitle() << " ";
	cout << setw(20) << leaf->GetTypeName() << " " << setw(10) << leaf->GetLen() << " ";
	cout << setw(10) << leaf->GetBranch()->GetBasketSize() << " " << setw(10) << leaf->GetBranch()->GetEntries() << " "; 
	cout << leaf->GetMaximum() << " ";
	cout << setw(20) << count_name << " ";
	//cout << setw(10) << val;
	cout << endl;
	
    }
    
    return 0;
}
