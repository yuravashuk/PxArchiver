using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace PxArchiverApp
{
    public partial class MetaEditor : Form
    {
        public List<PxFileMeta> metadata;

        public MetaEditor()
        {
            InitializeComponent();
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void removeButton_Click(object sender, EventArgs e)
        {
            int index = PropertyNames.SelectedIndex;
            metadata.RemoveAt(index);
            RefreshList();
        }

        private void addButton_Click(object sender, EventArgs e)
        {

        }

        private void RefreshList()
        {
            PropertyNames.Items.Clear();
            PropertyValues.Items.Clear();
            PropertyTypes.Items.Clear();

            foreach (var meta in metadata)
            {
                PropertyNames.Items.Add(meta.name);

                switch (meta.type)
                {
                    case MetaType.Boolean:
                        PropertyTypes.Items.Add("bool");
                        PropertyValues.Items.Add(meta.boolValue.ToString());
                        break;
                    case MetaType.Double:
                        PropertyTypes.Items.Add("double");
                        PropertyValues.Items.Add(meta.doubleValue.ToString());
                        break;
                    case MetaType.Float:
                        PropertyTypes.Items.Add("float");
                        PropertyValues.Items.Add(meta.floatValue.ToString());
                        break;
                    case MetaType.Int:
                        PropertyTypes.Items.Add("int");
                        PropertyValues.Items.Add(meta.intValue.ToString());
                        break;
                }
            }
        }
    }
}
