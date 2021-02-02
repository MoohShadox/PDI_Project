import dash
import dash_table
import dash_core_components as dcc
import dash_cytoscape as cyto
import dash_html_components as html
from dash.dependencies import Input, Output
from prp import *
from PRP1 import *
from PRP2 import *

external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']

app = dash.Dash(__name__, external_stylesheets=external_stylesheets)

test = "../PRP_instances/1PDI_Instance"

pathInst ="../PRP_instances/A_04"

prp = PRP(pathInst)

pathInput = dcc.Input(
            id="Path",
            type="text",
            placeholder="path to instance file",
            value=pathInst
        )

maxTimeStep = prp.l

def marks(m):

    marks=dict()

    for i in range(m+1):
        marks[i]=str(i)

    return marks

timeStep = dcc.Slider(
     id='timeStep',
    min=1,
    max=maxTimeStep,
    step=1,
    value=1,
    marks=marks(maxTimeStep)
    )

graph = html.Div(id="graph",children=prp.toDash(),
        style={"-moz-box-shadow":'inset 0 0 10px #000000',
                "-webkit-box-shadow": 'inset 0 0 10px',
                "box-shadow": 'inset 0 0 10px',
                "border-radius":"10px"})

fileButton = html.Button('Load Instance', id='fileButton',n_clicks=0)
prp1Button = html.Button('Load PRP1', id='prp1Button',n_clicks=0)
prp2Button = html.Button('Load PRP2', id='prp2Button',n_clicks=0)

interractions = html.Div([pathInput,fileButton,prp1Button,prp2Button,timeStep],style={"text-align":"center","justify-content":"center","align-content":"center"})

table = [{"QuantityProduced":1,"Inventory":2},{"QuantityProduced":1,"Inventory":2}]

prodT = dash_table.DataTable(
        id='prodTable',
        columns=[{"name": "Quantity produced", "id": "QuantityProduced"},{"name": "Inventory", "id": "Inventory"}],
        data=table,
)

app.layout = html.Div([html.H1(children='Visualisation',style={"text-align":"center","font-family":"sans-serif"}),graph,interractions,prodT])

@app.callback(
    dash.dependencies.Output('graph','children'),
    dash.dependencies.Output('prodTable','data'),
    [dash.dependencies.Input('fileButton','n_clicks')],
    [dash.dependencies.Input('prp1Button','n_clicks')],
    [dash.dependencies.Input('prp2Button','n_clicks')],
    [dash.dependencies.Input('timeStep', 'value')],
    [dash.dependencies.State('Path', 'value')])
def update_output(nc1,nc2,nc3,t,path):
    global pathInst,prp,table
    changed_id = [p['prop_id'] for p in dash.callback_context.triggered][0]
    pathInst=path
    if 'fileButton' in changed_id:
        prp=PRP(path)
        maxTimeStep = prp.l
    elif 'prp1Button' in changed_id:
        prp=PRP1Sol(path)
        table=prp.prodTable()
        maxTimeStep = prp.prp.l
    elif 'prp2Button' in changed_id:
        prp=PRP2Sol(path)
        maxTimeStep = prp.prp.l
    if path != None:
        return prp.toDash(t),table

if __name__ == '__main__':
    app.run_server(debug=True)
