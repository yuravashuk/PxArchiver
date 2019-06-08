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
    public partial class Form1 : Form
    {

        List<PxFileInfo> fileInfoes = new List<PxFileInfo>();

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            ArchiverProvider.CreateArchiver(1);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            ArchiverProvider.DestroyArchiver();
        }

        private void AddButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            openFileDialog1.Filter = "All files (*.*)|*.*";
            openFileDialog1.CheckFileExists = true;
            openFileDialog1.CheckPathExists = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                PxFileInfo info = new PxFileInfo();
                info.filePath = openFileDialog1.FileName;
                fileInfoes.Add(info);

                FilesList.Items.Add(openFileDialog1.FileName);
            }
        }

        private void RemoveButton_Click(object sender, EventArgs e)
        {
            try
            {
                int index = FilesList.SelectedIndex;
                fileInfoes.RemoveAt(index);
                FilesList.Items.RemoveAt(index);
            }
            catch
            {

            }
        }

        private void BuildButton_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveDialog = new SaveFileDialog();
            saveDialog.Filter = "Binary file (*.bin)|*.bin";
            saveDialog.CheckPathExists = true;

            if (saveDialog.ShowDialog() == DialogResult.OK)
            {
                foreach (String filePath in FilesList.Items)
                {
                    String fileName = System.IO.Path.GetFileNameWithoutExtension(filePath);
                    String fileExtension = System.IO.Path.GetExtension(filePath);
                    ArchiverProvider.AddFile(filePath, fileName, fileExtension);
                }

                ArchiverProvider.Save(saveDialog.FileName);
            }
        }

        private void ExportButton_Click(object sender, EventArgs e)
        {
            String filter = "Binary files (*.bin)|*.bin";

            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            openFileDialog1.Filter = filter;
            openFileDialog1.CheckFileExists = true;
            openFileDialog1.CheckPathExists = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                ArchiverProvider.Open(openFileDialog1.FileName);

                FolderBrowserDialog folderBrowserDialog1 = new FolderBrowserDialog();
                if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
                {
                    ArchiverProvider.Export(folderBrowserDialog1.SelectedPath);
                }
            }
        }

        private void AddFolder_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderDialog = new FolderBrowserDialog();
            if (folderDialog.ShowDialog() == DialogResult.OK)
            {
                String[] files = System.IO.Directory.GetFiles(folderDialog.SelectedPath);

                foreach (var fileName in files)
                {
                    PxFileInfo info = new PxFileInfo();
                    info.filePath = fileName;
                    fileInfoes.Add(info);
                    FilesList.Items.Add(fileName);
                }
            }
        }

        private void editMetadataButton_Click(object sender, EventArgs e)
        {
            MetaEditor editor = new MetaEditor();
            // 
            editor.ShowDialog();
        }

        private void FilesList_SelectedIndexChanged(object sender, EventArgs e)
        {
            int index = FilesList.SelectedIndex;

            if (index != -1)
            {
                languageTextbox.Text = fileInfoes[index].language.ToString();
                typeTextbox.Text = fileInfoes[index].type.ToString();
                compressedTextbox.Text = fileInfoes[index].compressed.ToString();
            }
        }

        private void saveOptionsButton_Click(object sender, EventArgs e)
        {
            int index = FilesList.SelectedIndex;

            if (index != -1)
            {
                int.TryParse(languageTextbox.Text, out fileInfoes[index].language);
                int.TryParse(typeTextbox.Text, out fileInfoes[index].type);
                int.TryParse(compressedTextbox.Text, out fileInfoes[index].compressed);
            }
        }
    }
}
