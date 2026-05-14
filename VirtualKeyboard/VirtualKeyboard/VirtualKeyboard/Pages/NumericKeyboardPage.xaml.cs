using CommunityToolkit.Mvvm.Messaging;
using VirtualKeyboard.Commands;
using VirtualKeyboard.Services;
using VirtualKeyboard.ViewModels;
namespace VirtualKeyboard.Pages;


public partial class NumericKeyboardPage : ContentPage
{
  
    public NumericKeyboardPage(NumericKeyboardViewModel viewModel)
	{
       
        InitializeComponent();
		BindingContext = viewModel;
        viewModel.Layout = Layouts.Numeric;
        SizeChanged += viewModel.SizeChanged;
        NavigatedTo += viewModel.NavigatedTo;
        NavigatedFrom += viewModel.NavigatedFrom;
    }

}