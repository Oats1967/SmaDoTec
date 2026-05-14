using CommunityToolkit.Mvvm.Messaging;
using VirtualKeyboard.Commands;
using VirtualKeyboard.Services;
using VirtualKeyboard.ViewModels;




namespace VirtualKeyboard.Pages;


public partial class GermanKeyboardPage : ContentPage 
{
    public GermanKeyboardPage(AlphabeticKeyboardViewModel viewModel)
	{
        InitializeComponent();
		BindingContext = viewModel;
        viewModel.Layout = Layouts.German;
        SizeChanged += viewModel.SizeChanged;
        NavigatedTo += viewModel.NavigatedTo;
       
        NavigatedFrom += viewModel.NavigatedFrom;
	}


}