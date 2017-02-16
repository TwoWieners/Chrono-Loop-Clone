﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LevelEditor
{
    public partial class ToolWindows : Form
    {

        public event EventHandler<BoolArgs> killMe;

        public ToolWindows()
        {
            InitializeComponent();
        }

        private void OnClosing(object sender, FormClosingEventArgs e)
        {
            killMe(this, new BoolArgs(false));
        }
    }
}
